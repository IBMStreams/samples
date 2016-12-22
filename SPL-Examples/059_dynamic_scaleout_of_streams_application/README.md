~~~~~~
/*
==========================================================================
First created on: Nov/28/2011
Last modified on: Aug/17/2013
 
This SPL composite is the self contained application that makes up
the price calculation engine. This composite provides the
code for importing the price calculation requests coming from 
the DynamicScaleout composite. It also calls the C++ primitive operator
that encapsulates the price calculation code. Finally, this
composite exports the result stream coming from the pricer
operator. Binary code for this composite can be deployed and run in
parallel as multiple pricing engine instances on several machines.
But, this example is configured to scale multiple pricing engines across
several CPU cores on a single machine. This can be easily reconfigured
to run on individual server machines.

 This application is built, run, and stopped using bash script files.
 For complete details about building, running, and stopping this application,
 please look inside the file named "readme.txt" available at the root of the 
 "dynamic_scaleout" project directory.
==========================================================================
*/
// Perl Arguments in effect for mixed-mode processing
// Argument count: 0

namespace com.ibm.streams.pricing;



composite Pricer {
	type
		pricerData = tuple<uint32 tickerSequenceNumber, uint32 targettedPricerId, uint32 respondingPricerId, 
			timestamp tickerEntryTime, int64 tickerResidencyTimeFromSourceToSink, timestamp applicationStartTime,
			int64 totalProcessingTimeForAllTickers, timestamp tickerEntryTimeInsideThePricer, 
			int64 priceCalculationTimeForThisTicker, rstring hostName, float32 spotPrice,
			float32 price, float32 delta, float32 gamma, float32 theta, float32 vega, float32 aoxb, float32 rho>;
			
	graph
		// Let us import the PricerInput stream that is exported by 
		// the DynamicScaleOut job meant to be received by specific pricers via an export operator property.
		// These exported streams will carry an export property that will carry the pricerId for which each
		// exported stream is targeted to. In the pricer code here, every pricer will hook with a 
		// particular stream that is exported for it by subscribing to its pricer id. Streams runtime
		// will do the magic of matching up every subscription with the corresponding exported stream.
		// The match key here is the pricer id.
		//
		// Note
		// ---- 
		// Pricer id for every worker pricer is dynamically assigned at the time when that pricer job
		// is submitted using the "streamtool submitjob" command. There is a -P option that allows 
		// the job submitter to pass any key=value combination to the job.
		// For example, in our case it will be -P pricerId=15
		stream <pricerData> ImportedPricerInput = Import(){
			param
				// We will dynamically change this subscription value during runtime when this pricer job is started.
				// Whoever is going to submit this pricer Streams job, he or she or that (a script!!!) must
				// provide a submissionTimeValue named "pricerId". (e-g: pricerId=15)
				subscription: pricerInputForPricerId == "DummyPricerId";
		} // End of Import

		// Let us have a Beacon to send a signal to change the input port import subscription.
		stream<int32 subscriptionChangeSignal> ImportSubscriptionChangeSignal = Beacon() {
			param
				initDelay: 3.0;
				iterations: 1;
		} // End of ImportSubscriptionChangeSignal = Beacon()

		// Let us dynamically change the stream import subscription value to the
		// current pricer id assigned via a job submission parameter.
		stream <ImportedPricerInput> PricerInput = Custom(ImportSubscriptionChangeSignal as ISCS;
			ImportedPricerInput as IPI) {
			logic
				onTuple ISCS: {
					// Let us change the import subscription value to the 
					// pricerId to this pricer assigned during application launch time.
					mutable rstring _newSubscription = 
						"pricerInputForPricerId == " + getSubmissionTimeValue("pricerId");
					// Please note the use of 1u for the second parameter. It denotes that
					// we want to change the import subscription for our input port 1 which
					// receives the imported pricer input stream.
					int32 rc = setInputPortImportSubscription(_newSubscription, 1u);
		
					if (rc != 0) {
						// Log the error and abort the application.
						appTrc(Trace.error, "New Subscription=" + _newSubscription);
						appTrc(Trace.error, "SetInputPortImportSubscription API failed with return code " + (rstring)rc);
						abort();
					} // End of if (rc != 0)				
				}	
				
				onTuple IPI: {
					// Simply forward this imported stream via the correct output stream.
					submit(IPI, PricerInput);
				}
		}

		// Let us add a timestamp to indicate when the ticker entered here for price calculation.
		// That will aid us in calculating the queuing delays in the upstream operators.
		stream<pricerData> TimeStampedPricerInput = Functor(PricerInput) {
			param
				filter: true;
				
			output
				TimeStampedPricerInput: tickerEntryTimeInsideThePricer = getTimestamp();
		} // End of TimeStampedPricerInput = Functor(PricerInput)

		// Inovoke the price calculation C++ primitive operator that in turn wraps the 
		// price calculation C++ code.
		stream<pricerData> PricerResult = DummyPricer(TimeStampedPricerInput) {
		} // End of PricerResult = DummyPricer(PricerInput)

		// Before we send the pricer results back to the downstream operator,
		// let us calculate the time it took inside the pricer logic. 		
		stream<pricerData> PricerFinalResult = Custom(PricerResult as FDR) {
			logic
				onTuple FDR: {
					mutable pricerData fddTuple = FDR;
					fddTuple.respondingPricerId = (uint32)getSubmissionTimeValue("pricerId");
					fddTuple.hostName = getConfiguredHostName();
				    fddTuple.priceCalculationTimeForThisTicker = ((getSeconds(getTimestamp()) * (int64)1000000000) + (int64)getNanoseconds(getTimestamp())) - 
				                    							 ((getSeconds(FDR.tickerEntryTimeInsideThePricer) * (int64)1000000000) + (int64)getNanoseconds(FDR.tickerEntryTimeInsideThePricer));
			    	submit(fddTuple, PricerFinalResult);
				} // End of onTuple FDR
		} // End of PricerFinalResult = Custom(PricerResult as FDR)

		// Export the pricerResult stream so that the DynamicScaleOut composite can receive it to
		// do further processing.
		() as ExportedPricerResult = Export(PricerFinalResult) {
			param
				properties: {pricerResult = "Done"};			
		} // End of Export(PricerFinalResult)

	config
		// Place all the operators in this composite in a single PE and
		// place that PE (which is generally known as the pricing engine) on a 
		// user-specified host.
		placement: partitionColocation("pricerEngine"), host("pricer_dummy_machine");
} // End of composite Pricer

~~~~~~

---
~~~~~~
/*
==========================================================================
First created on: Nov/28/2011
Last modified on: Aug/17/2013
 
This SPL composite is the test jig that drives the price
calculation engines running in parallel. This composite provides the
feeder (source), collector (sink), aggregation and performance
timing measurement logic. It works in concert with another SPL composite
named "com.ibm.streams.pricing.Pricer". This application is designed in such
a way to be elastic i.e. Application can be started with x number of 
pricer engines and then more pricer engines can be added or 
removed dynamically according to the resource availability as well as the 
demand for more pricer engines going up and down.
 
For this particular example application, Streams Studio is used only for
editing the project files and visualizing the running application using
the Streams instance graph. That is the reason why you will not see a
launch configuration for this project when you import this project into the 
Streams Studio. You need not create a launch configuration. Because,
this application is built, run, and stopped using bash script files.
For complete details about building, running, and stopping this application,
please look inside the file named readme.txt available at the root of the 
"dynamic_scaleout" project directory.
==========================================================================
*/
// Perl Arguments in effect for mixed-mode processing
// Argument count: 0

namespace com.ibm.streams.pricing.test;



composite DynamicScaleOut {
	type
		// Define the input tuple schema that will make its way through almost the entire application topology.
		pricerData = tuple<uint32 tickerSequenceNumber, uint32 targettedPricerId, uint32 respondingPricerId, 
			timestamp tickerEntryTime, int64 tickerResidencyTimeFromSourceToSink, timestamp applicationStartTime,
			int64 totalProcessingTimeForAllTickers, timestamp tickerEntryTimeInsideThePricer, 
			int64 priceCalculationTimeForThisTicker, rstring hostName, float32 spotPrice,
			float32 price, float32 delta, float32 gamma, float32 theta, float32 vega, float32 aoxb, float32 rho>;
		
		// Define the tuple schema which will carry the application performance collection values.	
		performanceData = tuple<int32 totalNumberOfPricerResults, int64 totalProcessingTimeForAllTickers, 
			int64 bestTickerProcessingTime, int64 worstTickerProcessingTime, int64 avergeTickerProcessingTime, 
			int64 bestPriceCalculationTime, int64 worstPriceCalculationTime, int64 avergePriceCalculationTime,
			float32 minPrice, float32 maxPrice, float32 averagePrice,
			float32 minVega, float32 maxVega, float32 averageVega>;

		// Define the tuple schema which will indicate the number of new pricer additions or the number of pricers going offline.
		pricerStartStopControlData = tuple<int32 numberOfPricersStartedOrStopped>;

	// Define the application graph below.	
	graph
		// Use a Beacon to kickstart the application by sending as many
		// price calculations that the user requested.
		// There will be a short initDelay required to hold off the
		// Beacon operator from sending tuples until all the pricing engines are started.
		// That initDelay value is specified via the bash script that is used to start the application.
		stream<pricerData> PricerGeneratedInput = Beacon() {
			param
				iterations: 1u;
				initDelay: 1f;		
			
			config
				// All the operators on the input feeder side will be partitioned into a single PE.
				placement: partitionColocation("PricerInput");
		} // End of PricerGeneratedInput = Beacon()
		
		// If we compile the application with LLM_RUM_IB, it happens that Beacon is continuously
		// engaging the downstream Custom operator. That somehow, doesn't allow the FileSource input
		// to go through the second input port of the Custom operator. That prevents us from adding
		// new pricers or removing existing pricers.
		// To resolve that issue, we are adding a Throttle to the Beacon output stream so that Custom operator
		// will have enough time to process the external control signals (start and stop pricers).
		stream<pricerData> ThrottledPricerGeneratedInput = Throttle(PricerGeneratedInput) {
			param
				precise: true;
				rate: 500.0;
				
			config
				// All the operators on the input feeder side will be partitioned into a single PE.
				placement: partitionColocation("PricerInput");
		} // End of Throttle(PricerGeneratedInput)
		
		// FileSource operator below will read a hot file to learn either about the availability of any newly
		// started pricer engines or about the unavailability of any active pricer engines that will soon be stopped.
		// Two External scripts will append to this hot file with an Integer value indicating
		// a positive number (engines started) or a negative number (engines to be stopped soon). Those two
		// scripts are start_new_pricers.sh, and stop_defactive_pricers.sh.
		stream<pricerStartStopControlData> PricerStartStopControlInput = FileSource() {
			param
				file: "/tmp/PricerStartStopControl.csv";
				format: csv;
				hasHeaderLine: true;
				hotFile: true;

			config
				placement: partitionColocation("PricerInput");
		} // End of PricerStartStopControlInput = FileSource()
		
		// Modify the generated input by assigning some of the tuple attribute values.
		// This operator receives two input streams.
		// 1) Pricer start stop control input data stream from the FileSource.
		// 2) Regular price calculation input data stream from the Beacon.
		stream<pricerData> PricerInput = Custom(PricerStartStopControlInput as PSSCI; ThrottledPricerGeneratedInput as FDGI) {
			logic
				// State variables for this operator are defined below.
				state: {
					mutable uint32 _tempTargettedPricerId = 0;
					mutable timestamp _applicationStartTime;
					mutable float32 _spotPrice = 0.0;
					mutable uint32 _tickerSequenceNumber = 0;
					mutable uint32 _totalPricersActiveAtThisTime = 1u;
				} // End of state
				
				// Process the newly arrived pricer start/stop control input tuple.
				onTuple PSSCI: {
					// External to this program, user has either already started more pricers or 
					// is going to stop some of the active pricers soon. That will be reflected via
					// a positive or negative number in the control input stream int32 field.
					// Let us adjust our count of total pricers active at this time.
					if (PSSCI.numberOfPricersStartedOrStopped >= 0) {
						// They have started additional pricer engines.
						appTrc(Trace.error, "Adding " +  (rstring)PSSCI.numberOfPricersStartedOrStopped + " pricer engine(s) to the current total of " + (rstring)_totalPricersActiveAtThisTime);
						_totalPricersActiveAtThisTime += (uint32)PSSCI.numberOfPricersStartedOrStopped;
						appTrc(Trace.error, "New count of total pricers:" + (rstring)_totalPricersActiveAtThisTime);
						appTrc(Trace.error, "Current ticker sequence number being scheduled for price calculation:" + (rstring)_tickerSequenceNumber);
						
						// If the user has started too many engines willy-nilly, let us set it to the grand total configured.
						// Because, we only have that many exported output ports configured for the current run.
						if (_totalPricersActiveAtThisTime > 1u) {
							_totalPricersActiveAtThisTime = 1u;
						}
					} else {
						// They are planning to stop x number of pricers that were started recently.
						// Since it is a negative number, it will get subtracted from the end of our active range of pricers.
						//
						// First, ensure that they are not planning to stop more number of pricers than what already are active at this time.
						if (_totalPricersActiveAtThisTime < (uint32)(-PSSCI.numberOfPricersStartedOrStopped)) {
							appTrc(Trace.error, "Fatal error: User is attempting to stop more pricer engines than that are active now.");
							appTrc(Trace.error, "Ignoring the request to stop invalid number of pricers.");
						} else if (_totalPricersActiveAtThisTime == (uint32)(-PSSCI.numberOfPricersStartedOrStopped)) {
							// We should have at least one pricer still running. If user is planning to stop all the
							// active pricers, then that is a fatal error. Let us log it and not stop any pricers.	
							appTrc(Trace.error, "Fatal error: User is attempting to stop all the active pricer engines.");	
							appTrc(Trace.error, "Ignoring the request to stop invalid number of pricers.");
						} else {
							appTrc(Trace.error, "Stopping " +  (rstring)(-PSSCI.numberOfPricersStartedOrStopped) + " pricer engine(s) from the current total of " + (rstring)_totalPricersActiveAtThisTime);
							_totalPricersActiveAtThisTime -= (uint32)(-PSSCI.numberOfPricersStartedOrStopped);
							appTrc(Trace.error, "New count of total pricers:" + (rstring)_totalPricersActiveAtThisTime);
							appTrc(Trace.error, "Current ticker sequence number being scheduled for price calculation:" + (rstring)_tickerSequenceNumber);
					    }
					}// End of if (PSSCI.numberOfPricersStartedOrStopped >= 0)
				} // End of onTuple PSSCI
				
				// Process the newly arrived pricer input tuple below.
				onTuple FDGI: {
					// Assign the received input tuple to a variable that carries the type of
					// the output stream of this operator.
					mutable PricerInput fdiTuple = ThrottledPricerGeneratedInput;
					
					if (_tempTargettedPricerId == 0u) {
						// We are starting the application. Let us record the start time.
						// This value will be assigned to every incoming tuple.
						// This attribute is useful to calculate the end-to-end time spent
						// in processing all the price calculation requests.
						// i.e. very first price calculation request to the very last request.
						_applicationStartTime = getTimestamp();
					} // End of if (_tempTargettedPricerId == 0u)
					
					// We will target one of the available pricers in a round-robin fashion.
					_tempTargettedPricerId++;
					
					if (_tempTargettedPricerId > _totalPricersActiveAtThisTime) {
						_tempTargettedPricerId = 1u;
					} // End of if (_tempTargettedPricerId > ...)
					
					// Set the intended pricer id so that we can verify if this task really 
					// gets done by the specified pricer id.
					fdiTuple.targettedPricerId = _tempTargettedPricerId;										
					// Set the ticker sequence number.
					fdiTuple.tickerSequenceNumber = ++_tickerSequenceNumber;

					// Let us increment the spotPrice by the amount specified by the user.
					// We will do that for every new tuple except for the very first tuple.
					if (_tickerSequenceNumber == 1u) {
						// Let us initialize the spotPrice to an initial price.
						_spotPrice = (float32)1;
					} else {
						_spotPrice += (float32)1;
					} // End of if (_tickerSequenceNumber == 1u)
					
					// Set the pricer operation start time.
					fdiTuple.tickerEntryTime = getTimestamp();
					fdiTuple.applicationStartTime = _applicationStartTime;
					// Assign the spot price for which we want to calculate the 
					// different values (option price, vega, gamma, rho, delta etc.)
					fdiTuple.spotPrice = _spotPrice;					
					// Send it away to a downstream operator.
					submit(fdiTuple, PricerInput);

					// Total number of spot prices that we need to calculate prices for is double the
					// size of what is actually needed.					
					// The first half of the specified total spot prices value is 
					// meant to warm-up the application components.
					// First half of the data goes through the normal application path and
					// any final results produced during this warm-up phase can be ignored.
					// After the warm-up phase is completed, the logic below will wait for
					// 10 seconds and then continue with the second half of the data.
					// During the second half of input data, all the results produced 
					// will be considered for analyzing the performance of the application.
					//
					//
					// In the following block, detect when we are going to cross from
					// the first half of the data transmission into the second half.
					// At that time, induce an artificial wait for 10 seconds.
					if (_tickerSequenceNumber == (uint32)(1/2)) {
						mutable uint32 time_t = second(getTimestamp());
						mutable int32 dummyInt = 0;
						
						// Wait until we fully waste the time for next 10 seconds.
						while ((second(getTimestamp()) - time_t) <= 10u) {
							// Do some dummy activity to pass the time.
							dummyInt += 456;
						} // End of while loop.
						
						// We are going to start the second half of the data block.
						// Let us set a new application start time that will be
						// used in calculating total time to process all the 
						// price calculation requests.
						_applicationStartTime = getTimestamp();
					} // End of if (_tickerSequenceNumber == ...
				} // End of onTuple FDGI

			config
				placement: partitionColocation("PricerInput");				
		} // End of PricerInput = Functor(ThrottledPricerGeneratedInput)
		
// We are going to export every individual PricerInput stream by targeting it to its specific pricer engine.
// It is done by adding an export property called pricerInputForPricerId. This property will carry the value of the
// pricer id to which this stream is being exported for. As the individual pricer jobs start at their own pace,
// they will subscribe for an exported stream that carries their particular pricer id. Streams runtime will
// connect them together. Isn't a great feature of Streams?
//
// Split the newly arrived price calculation request to a particular output stream so that
// it will be exported to the correct targetted pricing engine. 
(
		stream <PricerInput> PricerInput1) = Split(PricerInput as FDI) {
			param
				index: (int32)FDI.targettedPricerId - 1;
			
			config
				placement: partitionColocation("PricerInput");
		} // End of Custom(PricerInput)

// We will now export a property in the Export operator so that the pricer engines can connect properly to
// receive the input tuples being targetted to them for pricing calculations.

		() as ExportedPricerInput1 = Export(PricerInput1) {
			param
				properties: {pricerInputForPricerId=1};
		} // End of Export(PricerInput...)


		// ==========
		// Following section of this composite deals with the output side of the application.
		// ==========
		// Let us import the pricer result stream coming from every pricer engine running somewhere there in the world.
		stream <pricerData> PricerResult = Import() {
			param
				subscription: pricerResult == "Done";
		} // End of Import


		// Receive the results coming from individual pricers to a Custom operator to assign
		// a timestamp and then do logging when the very last result arrives here.
		stream <pricerData> TimeStampedPricerResult = Custom(PricerResult as PR) {
			logic
				onTuple PR: {
					mutable pricerData oTuple = PR;
					// Compute the time it took to perform the price calculation for this ticker.
					// i.e. since the generation of that price calculation request until that request
					// reaches here with the result coming from the pricing engine.
					oTuple.tickerResidencyTimeFromSourceToSink = ((getSeconds(getTimestamp()) * (int64)1000000000) + (int64)getNanoseconds(getTimestamp())) - 
																 ((getSeconds(PR.tickerEntryTime) * (int64)1000000000) + (int64)getNanoseconds(PR.tickerEntryTime));
					// This could be the pricer calculation for the very last spot price sent by the source operator.
					// With that assumption, let us compute the applicationProcessingTime attribute value.
					oTuple.totalProcessingTimeForAllTickers = ((getSeconds(getTimestamp()) * (int64)1000000000) + (int64)getNanoseconds(getTimestamp())) - 
															  ((getSeconds(PR.applicationStartTime) * (int64)1000000000) + (int64)getNanoseconds(PR.applicationStartTime));
					submit(oTuple, TimeStampedPricerResult);	
				} // End of onTuple PR

			config
				placement: partitionColocation("PricerOutput");
		} // End of TimeStampedPricerResult = Custom(PricerResult as PR)

		// Collect all the results coming from individual pricers to an aggregate operator to
		// compute certain performance characteristics.
		stream <performanceData> AggregatedPricerResult = Aggregate(TimeStampedPricerResult as TSPR) {
			window
				// Since results are being produced by the pricing engines for an
				// input data set that was sent in two equal-sized batches.
				// First batch of data is used for warming up the application
				// components; hence, results for the first batch may be ignored.
				// However, the results for the second batch is very important and
				// must not be ignored.
				// We will set the tumbling window size to match the two equal-sized
				// batches. That is why, we are dividing the total spot prices by 2.
				TSPR: tumbling, count(1/2);
			
			output
				// Compute various aggregation values.
				AggregatedPricerResult: totalNumberOfPricerResults = CountAll(),
										totalProcessingTimeForAllTickers = Max(totalProcessingTimeForAllTickers),
										bestTickerProcessingTime = Min(tickerResidencyTimeFromSourceToSink),
										worstTickerProcessingTime = Max(tickerResidencyTimeFromSourceToSink),
										avergeTickerProcessingTime = Average(tickerResidencyTimeFromSourceToSink),
										bestPriceCalculationTime = Min(priceCalculationTimeForThisTicker),
										worstPriceCalculationTime = Max(priceCalculationTimeForThisTicker),
										avergePriceCalculationTime = Average(priceCalculationTimeForThisTicker),
										minPrice = Min(price),
										maxPrice = Max(price),
										averagePrice = Average(price),
										minVega = Min(vega),
										maxVega = Max(vega),
										averageVega = Average(vega);

			config
				placement: partitionColocation("PricerOutput");
		} // End of AggregatedPricerResult = Aggregate(TimeStampedPricerResult)

		// Write the aggregated results to a Sink output file.
		// Let us write it in the /tmp directory located on a local hard drive. 
		// If we don't do that, it will write that log file in the application's
		// date sub-directory, which is usually located on an NFS mounted volume.
		// Writing to an NFS directory will be very slow. That is why /tmp is preferred.
		() as FileWriter = FileSink(AggregatedPricerResult) {
			param
				format: csv;
				file: "/tmp/Pricer-Performance-Result.csv";
				flush: 1u;

			config
				placement: partitionColocation("PricerOutput");
		} // End of FileWriter = FileSink(AggregatedPricerResult)

/*
 		// Use the following Functor and FileSink operators if you want to
 		// log the individual tickerResidencyTimeFromSourceToSink value for 
 		// every ticker being sent for price calculation.
		stream <uint32 tickerSequenceNumber, int64 tickerProcessingTime, rstring hostName> TickerProcessingTime = Functor(TimeStampedPricerResult as TSPR) {
			output
				TickerProcessingTime: tickerSequenceNumber = TSPR.tickerSequenceNumber,
					tickerProcessingTime = TSPR.tickerResidencyTimeFromSourceToSink,
					hostName = TSPR.hostName;

			config
				placement: partitionColocation("PricerOutput");
		} // End of FileWriter = FileSink(AggregatedPricerResult)

		() as FileWriter2 = FileSink(TickerProcessingTime) {
			param
				format: csv;
				file: "/tmp/Ticker-Processing-Time-Result.csv";
				flush: 1u;

			config
				placement: partitionColocation("PricerOutput");
		} // End of FileWriter2 = FileSink(TickerProcessingTime)
*/

	// Place all the PEs in this composite on a single node.
	config
		// Place all the operators in this composite in a single PE and
		// place that PE on a user-specified host.
		placement: host("pricer_dummy_machine");
} // End of composite DynamicScaleOut

~~~~~~
