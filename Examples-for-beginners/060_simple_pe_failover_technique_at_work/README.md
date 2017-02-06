~~~~~~ Scala 
/*
==========================================================================
First created on: Aug/18/2013
Last modified on: Mar/29/2015

READ THIS FULL COMMENTARY BEFORE RUNNING THIS EXAMPLE
-----------------------------------------------------
This example shows a simple technique that Streams developers may
use to provide fail-over support for their important analytic operators.
Such a fail-over technique is purely done at the application-level by
the Streams application developers. Readers should note that it is 
entirely different from any other sophisticated PE fail-over/restart
features that may be provided in the Streams product out-of-the-box right now or
in the future.  

To level-set, let us look at a common definition of the term "Fail-Over".
Thanks to the wonderful people here: http://en.wikipedia.org/wiki/Failover
 
"In computing, fail-over is switching to a redundant or standby computer server,
system, hardware component or network upon the failure or abnormal termination of
the previously active application, server, system, hardware component, or network.
fail-over and switch-over are essentially the same operation, except that fail-over is
automatic and usually operates without warning, while switch-over requires human intervention.

Systems designers usually provide fail-over capability in servers, systems or networks requiring
continuous availability -the used term is High Availability- and a high degree of reliability.

At server level, fail-over automation usually uses a "heart beat" cable that connects two servers.
As long as a regular "pulse" or "heart beat" continues between the main server and the second server,
the second server will not initiate its systems. There may also be a third "spare parts" server that
has running spare components for "hot" switching to prevent down-time. The second server takes over
the work of the first as soon as it detects an alteration in the "heart beat" of the first machine.
Some systems have the ability to send a notification of fail-over." 

This example faithfully implements the classic definition of fail-over as stated above. We merely
show here a basic idea so that the Streams developers can start thinking deeply about this topic.
If this fail-over approach is not an elegant one for your needs, please ignore this example.

Doing this for a C++ or Java primitive operators
------------------------------------------------
SPL code below shows the fail-over of an analytics operator written fully inside a SPL Custom operator.
If you want to do fail-over for a C++/Java primitive operator, you can use a similar technical approach by launching
your C++/Java operator with two copies (primary and secondary). Then, you can change the code below to fuse
the two Custom operators with each of your C++/Java primitive operator instances and slightly modify the code for 
failure detection, take-over, results caching, and pending results retransmission etc. We leave all of that to you
for a homework exercise. Try to implement that and validate the results. In principle, it should work well.
If not, please contact us and we will brainstorm more.

CAUTION
-------
Please don't run this example code without thoroughly understanding what it exactly does.
When you run this example, it will create a very big results Sink file in this project's data directory.
If you keep running this example non-stop for a long time, you will eventually fill up your hard disk.
To avoid that situation, run this example very briefly to test the fail-over and stop this
application as soon as you are convinced that this particular approach taken for the fail-over works.

Steps to test this fail-over example
------------------------------------
1) Read this SPL code and fully understand what it does.

2) Compile this example in distributed mode and run this example.

3) Using the Streams Instance Graph or Streams web console, you can watch the deployed application topology.
   You will see two identical PEs doing the same analytics. (one primary and one secondary).
   If you hover your mouse over the primary PE, you will see the tuple count keep increasing on both of its output ports.
   If you hover your mouse over the secondary PE, you will see the tuple count staying at 0 on both of its output ports.

4) From an xterm window, run this command to get results as shown below:   streamtool lspes -d <YOUR_DOMAIN_NAME> -i <YOUR_INSTANCE_NAME> -l

Instance: StreamsInstance
  Id State   RC Healthy Resource               PID   JobId JobName                                                        Operators
   0 Running -  yes     streamsqse.localdomain 21660     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 CalcResultFrom2
   1 Running -  yes     streamsqse.localdomain 21558     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 CalcRequestSignal,ThrottledCalcRequestSignal,CalcRequest
   2 Running -  yes     streamsqse.localdomain 21657     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 CalcResultFrom1
   3 Running -  yes     streamsqse.localdomain 21655     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 HeartBeatReminder
   4 Running -  yes     streamsqse.localdomain 21659     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 ResultsSink
 

5) At the program start-up, a PE named "CalcResultFrom1" is playing the role of a primary PE and
   a PE named "CalcResultFrom2" is playing the role of a secondary PE.

6) View the PE application log (pe.XX.out) for both the primary and secondary PEs.
   You should see something like this.
   
   For the primary PE, you should see this log line:
      At the application start-up, I'm being assigned the role of a primary PE. [1]
   
   For the secondary PE, you should see this log line:
      At the application start-up, I'm being assigned the role of a secondary PE. [1]

7) In the xterm window, you can kill the primary PE.
   kill -9 21657

8) If you perform the command you did in step (4) now, you will see this result with a new process id for
   what used to be primary PE before.

  Id State   RC Healthy Resource               PID   JobId JobName                                                        Operators
   0 Running -  yes     streamsqse.localdomain 21660     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 CalcResultFrom2
   1 Running -  yes     streamsqse.localdomain 21558     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 CalcRequestSignal,ThrottledCalcRequestSignal,CalcRequest
   2 Running -  yes     streamsqse.localdomain 23573     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 CalcResultFrom1
   3 Running -  yes     streamsqse.localdomain 21655     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 HeartBeatReminder
   4 Running -  yes     streamsqse.localdomain 21659     0 com.acme.failover.test::simple_pe_failover_technique_at_work_0 ResultsSink
 
   When you abruptly killed the primary PE, our Streams runtime automatically restarted that PE and changed its role as a secondary PE.
   At the same time, it changed the role of the existing secondary PE (with PID 21660) to be a primary PE to continue the analytics
   operations normally as if nothing happened.

9) If you perform step (3) now, you will see the role reversal. Current primary PE (formerly secondary) is pumping data on
   both of its output ports, whereas the current secondary PE (formerly primary) is having a tuple count of 0 on both of its output ports.
   What this means is that we have the fail-over operation working correctly for the chosen analytics operator.

10) If you perform step (6), you will see something like this.

   For the new primary PE, you should see this new log line:
      Detected a primary PE failure. I'm changing my role from being a secondary PE to a primary PE. [12398]

   For the secondary PE, you should see this log line:
       I crashed and got restarted. I will serve as a secondary PE. [12389]
 
11) You can also view the console log (pec.peXX.stdouterr) for the ResultsSink PE to see if there are
    log messages about any missing or duplicate tuples logged due to the fail-over that occurred just now.
    (In general, longer the heart beat interval, it will take more time to detect the primary PE failure and
     do a role change-over. With such long heart beat intervals, there is higher chance for the new primary PE to
     to send a few duplicate tuples due to the delayed role change-over. As shown in the FileSink code below,
     it is easy to detect any duplicate tuples and simply throw them away.)

12) In the application data directory, you can also view Calc-Results1.csv file to see that
    the final analytics output is correct in the exact order in which the input tuples were submitted.

13) You can now kill the current primary PE again and see that the current secondary PE comes for rescue instantly.

14) You can stop the application now using the "streamtool canceljob" command.

15) Delete the data/Calc-Results1.csv which may have grown to be a big sized file.
==========================================================================
*/
// Perl Arguments in effect for mixed-mode processing
// Argument count: 0

namespace com.acme.failover.test;

composite simple_pe_failover_technique_at_work {
	param
		// As a guideline, you should try different values for the next four constants to arrive at the
		// best combination for your hardware/network environment and your application scenario.
		expression<float64> $HEART_BEAT_INTERVAL_IN_SECONDS : 0.025; // It is every 25 milliseconds.
		expression<int32> $ALLOWED_NUMBER_OF_MISSED_HEART_BEATS_BEFORE_FAIL_OVER : 10; 
		expression<int32> $EXTRA_MISSED_HEART_BEAT_CYCLES_ALLOWED_FOR_A_RESTARTED_PE : 4;
		expression<float64> $THROTTLING_RATE_FOR_INCOMING_TUPLES : 200.0; // It is X per second
		expression<int8> $PRIMARY_OPERATOR : (int8)1;
		expression<int8> $SECONDARY_OPERATOR : (int8)2;
		
	type
		calc_data = tuple<uint64 calcRequestSequenceNumber, uint64 operand1, uint64 operand2, list<uint64> calcResults>;
		heart_beat_msg = tuple<uint64 lastSequenceNumberProcessed>;
	
	graph
		// We will have a Beacon that will keep generating arithmetic calculation request signals.
		// In real life, this will be replaced by the input stream coming for a specific analytic operator that
		// you want to provision for fail-over. 
		stream<calc_data> CalcRequestSignal = Beacon() {
			param
				initDelay: 7.0;

			config
				placement: partitionColocation("Calc_Request_Generator");
		}

		// Since we are continuously creating the analytic request signals using the Beacon operator,
		// it will bombard the analytic operators at a crazy data tuple rate. That is not realistic. 
		// Let us throttle the input data tuple rate to a more practical amount that we need.
		// You can change the data tuple rate value to suit your needs.
		// Note: If you need a higher data rate, then you have to increase the value of
		// $ALLOWED_NUMBER_OF_MISSED_HEART_BEATS_BEFORE_FAIL_OVER. You have to do more
		// tests and come up with the correct combination of those two values.
		// In our lab tests, we noticed that compiling this application with "-F LLM_RUM_TCP" option seems to
		// help this particular fail-over technique work well for higher data rates.
		stream<calc_data> ThrottledCalcRequestSignal = Throttle(CalcRequestSignal) {
			param
				precise: true;
				rate: $THROTTLING_RATE_FOR_INCOMING_TUPLES;
				
			config
				// All the operators on the input data feeder side will be partitioned into a single PE.
				placement: partitionColocation("Calc_Request_Generator");
		}
		
		// We will receive the signals from the beacon and form the actual calculation requests.
		// We will send this request to be processed by two different instances of the same 
		// calculation analytic operator we are going to build below.
		//
		// 1) First instance will be a primary analytic operator
		// 2) Second instance will be a secondary analytic operator
		stream<calc_data> CalcRequest  = Custom(ThrottledCalcRequestSignal as TCRS) {
			logic
				state: {
					mutable uint64 _cnt = 0;
					mutable uint64 _operand1 = 1;
					mutable uint64 _operand2 = 0;
				}
				
				onTuple TCRS: {
					_operand2 = _operand1;
					_operand1++;
					mutable calc_data oTuple = {};
					oTuple.calcRequestSequenceNumber = ++_cnt;
					oTuple.operand1 = _operand1;
					oTuple.operand2 = _operand2;
					// Send the calculation request. This request will be tapped by both
					// the primary and the secondary analytic operators.
					submit(oTuple, CalcRequest);
				}
				
			config
				placement: partitionColocation("Calc_Request_Generator");
				threadedPort: queue(TCRS, Sys.Wait);
		}
		
		// Let us create a beacon that will keep pumping signals at a pre-configured time interval.
		// These signals are created to alert/remind the primary calculator to send a heart beat to its
		// secondary calculator (OR) to alert/remind the secondary calculator to detect whether the primary has failed.
		// As part of this reminder, we will simply keep sending the number of HB reminders we have
		// sent so far. This value has a special meaning, which you will see in the downstream operator below.
		stream<uint64 hbrCnt> HeartBeatReminder = Beacon() {
			logic
				state: {
					mutable uint64 _hbrCnt = 0;
				}
			
			param
				period: $HEART_BEAT_INTERVAL_IN_SECONDS;
				initDelay: 4.0;
				
			output
				HeartBeatReminder: hbrCnt = ++_hbrCnt;  
		}
		 
		// Let us create two identical copies of our calculator analytic operator.
		// You can choose to run both of them as individual PEs either on a single machine or
		// distribute them across two different machines for protection against process & machine failures.
		//
		// 1) First copy will be a primary analytic operator
		// 2) Second copy will be a secondary analytic operator
		//
		// During the normal operation, both primary and secondary analytic operators will receive the
		// same incoming tuple that carries a unique sequence number. Both primary and secondary will
		// perform the exact business logic for every incoming tuple and arrive at identical results.
		// However, only the primary will send its results to a downstream operator.
		// Secondary will perform the same logic and arrive at the same result and then simply
		// throw away the produced results without sending it via its output port.

		// Receive three input streams:
		// 1) Periodic heart beat REMINDER for a primary PE to send out its
		//    heart beat to the other secondary analytic operator that is part of the primary/secondary pair.
		// 2) Heart beat sent from the other analytic operator that is part of the primary/secondary pair.
		// 3) Analytic Request (In our case, a simple request to make a couple of arithmetic operations.)
		(stream<calc_data> CalcResultFrom1 as Result; stream<heart_beat_msg> HeartBeatFrom1 as HBO) = 
			Custom(HeartBeatReminder as HBR;
				HeartBeatFrom2 as HBI; 
				CalcRequest as CR) {
				logic
					state: {
						// A value of 1 for this state variable means it is a primary analytic operator.
						// A value of 2 for this state variable means it is a secondary analytic operator. 
						mutable int8 _primarySecondaryIdentifier = (int8)1;
						// We will store the current reminder count sent by the HBR beacon operator.
						mutable uint64 _hbrCnt = 0ul;
						mutable int32 _missedHeartBeatCnt = 0;
						mutable heart_beat_msg _heartBeatMsg = {};
						mutable uint64 _lastSequenceNumberProcessedByThePrimary = 0ul;
						mutable list<calc_data> _analyticsResultTuplesSinceTheLastHeartBeat = [];
						
					}
					
					onTuple HBR: {
						// This is a heart beat reminder for us to do the following.
						// 1) If we are configured as a primary operator, we should send a heart beat to inform that we are alive and well.
						// 2) If we are configured a secondary operator, we should check if we have been getting the heart beat routinely
						//    from the primary operator. If it is not so, then it is time for the secondary to take-over and become the new primary.
						//
						// There are few more important discussion points surrounding the primary/secondary role assignments.
						//  
						// 1) During the application start-up, a PE with a _primarySecondaryIdentifier set to 1 will act as the primary.
						// At the same time, a PE with a _primarySecondaryIdentifier set to 2 will act as the secondary. (Both primary and
						// secondary roles are assigned as it was originally programmed.)
						// 
						// 2) When a fail-over takes place during the failure of the primary PE, a secondary PE will
						// change its role as the primary PE. (In this situation, only the primary role is being played. 
						// Secondary PE role is absent for a brief period during this time.)
						//
						// 3) When the failed primary or secondary PE is restarted, it must check if it is coming alive again after
						//    a failure. If yes, then it must configure itself to play the role of a secondary. That is because, 
						//    there must already be a primary PE active all the time. (Both primary and secondary roles are 
						//    being played either as it was originally programmed or after a role reversal.)
						//  
						if (HBR.hbrCnt == 1ul) {
							// This is the very first heart beat reminder. Let us log the initial roles being 
							// played by our primary/secondary PE pair.
							if (_primarySecondaryIdentifier == $PRIMARY_OPERATOR) {
								appTrc(Trace.error, "At the time of the application start-up, I'm doing the role of a primary PE. [" + (rstring)HBR.hbrCnt + "]");
							} else {
								appTrc(Trace.error, "At the time of the application start-up, I'm doing the role of a secondary PE. [" + (rstring)HBR.hbrCnt + "]");
							}
						}
						
						if (HBR.hbrCnt > _hbrCnt+1ul) { 							
							// Ideally, HBR.hbrCnt coming from the Beacon should be higher than _hbrCnt exactly by 1.
							// If we see the HBR.hbrCnt to be much higher than _hbrCnt+1, that means HBR has been
							// active for a while and this analytic PE is arriving here later because of an 
							// automatic or a manual restart after a failure.
							// There must be another active PE that has already taken over as a primary.
							// Let us make this newly restarted PE to play the role of a secondary PE.
							_primarySecondaryIdentifier = $SECONDARY_OPERATOR;
							// Since this PE was restarted and assigned a role to be a secondary, let it wait for
							// a few more extra cycles before doing its duty to check for heart beat misses.
							_missedHeartBeatCnt = 0 - ($ALLOWED_NUMBER_OF_MISSED_HEART_BEATS_BEFORE_FAIL_OVER * $EXTRA_MISSED_HEART_BEAT_CYCLES_ALLOWED_FOR_A_RESTARTED_PE);
							appTrc(Trace.error, "I crashed and got restarted. I will serve as a secondary PE. [" + (rstring)HBR.hbrCnt + "]");
						}
						
						// Record the current _hbrCnt. It is important for detecting a condition of the failed Primary PE coming alive after a restart.
						_hbrCnt = HBR.hbrCnt;
						
						// If this is a secondary PE, then it has to check whether the primary PE is operating normally.
						if (_primarySecondaryIdentifier == $SECONDARY_OPERATOR) {
							// Simply increment the missed heart beat count by 1.
							// This will get reset to 0 later by the actual heart beat arriving from the primary.
							if (++_missedHeartBeatCnt > $ALLOWED_NUMBER_OF_MISSED_HEART_BEATS_BEFORE_FAIL_OVER) {
								// It looks like the Primary PE has failed.
								// Let us make the current secondary PE to become the primary PE.
								_missedHeartBeatCnt = 0;
								_primarySecondaryIdentifier = $PRIMARY_OPERATOR;
								appTrc(Trace.error, "Detected a primary PE failure. I'm changing my role from being a secondary PE to a primary PE. [" + (rstring)_hbrCnt + "]");
								
								// We have cached all the result tuples that the secondary PE has processed since the 
								// last successful heart beat exchange. All along, our secondary PE has been simply caching
								// the analytic results without submitting the results.
								// Let us submit all the cached tuples to the downstream operator(s) starting from one more than the 
								// last known sequence number that the primary PE successfully processed before going down (crashing). 
								mutable uint64 _tmpSequenceNumber = 0ul;
								for (calc_data _result in _analyticsResultTuplesSinceTheLastHeartBeat) {
									_tmpSequenceNumber = _result.calcRequestSequenceNumber;
								
									// Let us submit any analytic result computed for sequence numbers higher than
									// the last known sequence number that was successfully processed by the primary PE before it failed.
									if (_tmpSequenceNumber > _lastSequenceNumberProcessedByThePrimary) {
										submit(_result, Result);
									}
								} 
								
								// Set the new last sequence number processed by this new primary that took over the operations during fail-over.
								if (_tmpSequenceNumber > 0ul) {
									_lastSequenceNumberProcessedByThePrimary = _tmpSequenceNumber;
								}
								
								// We switched our role to become a primary PE.
								// In this new role, we are not going to be concerned at all about maintaining an analyticsResultTuples cache.
								// we can clear that cache now. 
								clearM(_analyticsResultTuplesSinceTheLastHeartBeat);
							}
						}
						
						// If this is a primary PE, send a heart beat now.
						if (_primarySecondaryIdentifier == $PRIMARY_OPERATOR) {
							_heartBeatMsg.lastSequenceNumberProcessed = _lastSequenceNumberProcessedByThePrimary;
							submit(_heartBeatMsg, HBO);
						}
					}
				
				// Process the heart beat input.
				onTuple HBI: {
					// If this instance of the operator is configured as the secondary PE, then it will
					// periodically receive HB from the primary PE during normal operation.
					// Let us simply record the last sequence number processed by the Primary. We will
					// use it when the secondary PE needs to take over as primary during the failure of our current primary.
					_lastSequenceNumberProcessedByThePrimary = HBI.lastSequenceNumberProcessed;
					// Since we received a heart beat from the primary, everything is normal at this time.
					// Let us reset the missed heart beat count.
					_missedHeartBeatCnt = 0;
					// Let us also clear our results cache.
					clearM(_analyticsResultTuplesSinceTheLastHeartBeat);
				} 
				
				// Let us process the actual analytics request.
				onTuple CR: {
					// Irrespective of whether we are a primary or a secondary, we will process
					// the actual analytics request and prepare the result.
					// In this example, our analytics is so simple. We will do an addition and a multiplication.
					clearM(CR.calcResults);
					appendM(CR.calcResults, CR.operand1 + CR.operand2);
					appendM(CR.calcResults, CR.operand1 * CR.operand2);
					
					// If it is a primary PE, we will submit it.
					// Please note that we are doing a check to see that we have received at least
					// one heart beat reminder. This will prevent a newly restarted former failed primary PE from
					// beginning to process the analytics requests again. 
					if ((_primarySecondaryIdentifier == $PRIMARY_OPERATOR) && (_hbrCnt > 0ul)) {
						submit(CR, Result);
						_lastSequenceNumberProcessedByThePrimary = CR.calcRequestSequenceNumber;
					} else if ((_primarySecondaryIdentifier == $SECONDARY_OPERATOR) && (_missedHeartBeatCnt >= 0)) {
						// If it is a secondary PE, we will simply store the results in a local cache.
						// In case of a current primary PE's failure and when the current
						// secondary PE takes over control and becomes the new primary, we will
						// transmit all the result tuples from the point at which the failure happened.
						//
						// Missed heart beat count for a secondary PE will be less than 0 for a brief period when
						// a failed primary PE is restarted with a new role as a secondary PE. (see the logic for onTuple HBR)
						// During that brief period, we will not cache anything in the results cache as indicated in the "else if" condition above.
						appendM(_analyticsResultTuplesSinceTheLastHeartBeat, CR);
					} 
				}

			config
				// If needed, you can also isolate the primary and secondary by placing them
				// on two different machines. You can add a host section to the following
				// placement configuration with a combination of PERL and SPL.
				placement: partitionColocation("Analytics_Operator_1");
				restartable: true;
				relocatable: true;
		}


		// Receive three input streams:
		// 1) Periodic heart beat REMINDER for a primary PE to send out its
		//    heart beat to the other secondary analytic operator that is part of the primary/secondary pair.
		// 2) Heart beat sent from the other analytic operator that is part of the primary/secondary pair.
		// 3) Analytic Request (In our case, a simple request to make a couple of arithmetic operations.)
		(stream<calc_data> CalcResultFrom2 as Result; stream<heart_beat_msg> HeartBeatFrom2 as HBO) = 
			Custom(HeartBeatReminder as HBR;
				HeartBeatFrom1 as HBI; 
				CalcRequest as CR) {
				logic
					state: {
						// A value of 1 for this state variable means it is a primary analytic operator.
						// A value of 2 for this state variable means it is a secondary analytic operator. 
						mutable int8 _primarySecondaryIdentifier = (int8)2;
						// We will store the current reminder count sent by the HBR beacon operator.
						mutable uint64 _hbrCnt = 0ul;
						mutable int32 _missedHeartBeatCnt = 0;
						mutable heart_beat_msg _heartBeatMsg = {};
						mutable uint64 _lastSequenceNumberProcessedByThePrimary = 0ul;
						mutable list<calc_data> _analyticsResultTuplesSinceTheLastHeartBeat = [];
						
					}
					
					onTuple HBR: {
						// This is a heart beat reminder for us to do the following.
						// 1) If we are configured as a primary operator, we should send a heart beat to inform that we are alive and well.
						// 2) If we are configured a secondary operator, we should check if we have been getting the heart beat routinely
						//    from the primary operator. If it is not so, then it is time for the secondary to take-over and become the new primary.
						//
						// There are few more important discussion points surrounding the primary/secondary role assignments.
						//  
						// 1) During the application start-up, a PE with a _primarySecondaryIdentifier set to 1 will act as the primary.
						// At the same time, a PE with a _primarySecondaryIdentifier set to 2 will act as the secondary. (Both primary and
						// secondary roles are assigned as it was originally programmed.)
						// 
						// 2) When a fail-over takes place during the failure of the primary PE, a secondary PE will
						// change its role as the primary PE. (In this situation, only the primary role is being played. 
						// Secondary PE role is absent for a brief period during this time.)
						//
						// 3) When the failed primary or secondary PE is restarted, it must check if it is coming alive again after
						//    a failure. If yes, then it must configure itself to play the role of a secondary. That is because, 
						//    there must already be a primary PE active all the time. (Both primary and secondary roles are 
						//    being played either as it was originally programmed or after a role reversal.)
						//  
						if (HBR.hbrCnt == 1ul) {
							// This is the very first heart beat reminder. Let us log the initial roles being 
							// played by our primary/secondary PE pair.
							if (_primarySecondaryIdentifier == $PRIMARY_OPERATOR) {
								appTrc(Trace.error, "At the time of the application start-up, I'm doing the role of a primary PE. [" + (rstring)HBR.hbrCnt + "]");
							} else {
								appTrc(Trace.error, "At the time of the application start-up, I'm doing the role of a secondary PE. [" + (rstring)HBR.hbrCnt + "]");
							}
						}
						
						if (HBR.hbrCnt > _hbrCnt+1ul) { 							
							// Ideally, HBR.hbrCnt coming from the Beacon should be higher than _hbrCnt exactly by 1.
							// If we see the HBR.hbrCnt to be much higher than _hbrCnt+1, that means HBR has been
							// active for a while and this analytic PE is arriving here later because of an 
							// automatic or a manual restart after a failure.
							// There must be another active PE that has already taken over as a primary.
							// Let us make this newly restarted PE to play the role of a secondary PE.
							_primarySecondaryIdentifier = $SECONDARY_OPERATOR;
							// Since this PE was restarted and assigned a role to be a secondary, let it wait for
							// a few more extra cycles before doing its duty to check for heart beat misses.
							_missedHeartBeatCnt = 0 - ($ALLOWED_NUMBER_OF_MISSED_HEART_BEATS_BEFORE_FAIL_OVER * $EXTRA_MISSED_HEART_BEAT_CYCLES_ALLOWED_FOR_A_RESTARTED_PE);
							appTrc(Trace.error, "I crashed and got restarted. I will serve as a secondary PE. [" + (rstring)HBR.hbrCnt + "]");
						}
						
						// Record the current _hbrCnt. It is important for detecting a condition of the failed Primary PE coming alive after a restart.
						_hbrCnt = HBR.hbrCnt;
						
						// If this is a secondary PE, then it has to check whether the primary PE is operating normally.
						if (_primarySecondaryIdentifier == $SECONDARY_OPERATOR) {
							// Simply increment the missed heart beat count by 1.
							// This will get reset to 0 later by the actual heart beat arriving from the primary.
							if (++_missedHeartBeatCnt > $ALLOWED_NUMBER_OF_MISSED_HEART_BEATS_BEFORE_FAIL_OVER) {
								// It looks like the Primary PE has failed.
								// Let us make the current secondary PE to become the primary PE.
								_missedHeartBeatCnt = 0;
								_primarySecondaryIdentifier = $PRIMARY_OPERATOR;
								appTrc(Trace.error, "Detected a primary PE failure. I'm changing my role from being a secondary PE to a primary PE. [" + (rstring)_hbrCnt + "]");
								
								// We have cached all the result tuples that the secondary PE has processed since the 
								// last successful heart beat exchange. All along, our secondary PE has been simply caching
								// the analytic results without submitting the results.
								// Let us submit all the cached tuples to the downstream operator(s) starting from one more than the 
								// last known sequence number that the primary PE successfully processed before going down (crashing). 
								mutable uint64 _tmpSequenceNumber = 0ul;
								for (calc_data _result in _analyticsResultTuplesSinceTheLastHeartBeat) {
									_tmpSequenceNumber = _result.calcRequestSequenceNumber;
								
									// Let us submit any analytic result computed for sequence numbers higher than
									// the last known sequence number that was successfully processed by the primary PE before it failed.
									if (_tmpSequenceNumber > _lastSequenceNumberProcessedByThePrimary) {
										submit(_result, Result);
									}
								} 
								
								// Set the new last sequence number processed by this new primary that took over the operations during fail-over.
								if (_tmpSequenceNumber > 0ul) {
									_lastSequenceNumberProcessedByThePrimary = _tmpSequenceNumber;
								}
								
								// We switched our role to become a primary PE.
								// In this new role, we are not going to be concerned at all about maintaining an analyticsResultTuples cache.
								// we can clear that cache now. 
								clearM(_analyticsResultTuplesSinceTheLastHeartBeat);
							}
						}
						
						// If this is a primary PE, send a heart beat now.
						if (_primarySecondaryIdentifier == $PRIMARY_OPERATOR) {
							_heartBeatMsg.lastSequenceNumberProcessed = _lastSequenceNumberProcessedByThePrimary;
							submit(_heartBeatMsg, HBO);
						}
					}
				
				// Process the heart beat input.
				onTuple HBI: {
					// If this instance of the operator is configured as the secondary PE, then it will
					// periodically receive HB from the primary PE during normal operation.
					// Let us simply record the last sequence number processed by the Primary. We will
					// use it when the secondary PE needs to take over as primary during the failure of our current primary.
					_lastSequenceNumberProcessedByThePrimary = HBI.lastSequenceNumberProcessed;
					// Since we received a heart beat from the primary, everything is normal at this time.
					// Let us reset the missed heart beat count.
					_missedHeartBeatCnt = 0;
					// Let us also clear our results cache.
					clearM(_analyticsResultTuplesSinceTheLastHeartBeat);
				} 
				
				// Let us process the actual analytics request.
				onTuple CR: {
					// Irrespective of whether we are a primary or a secondary, we will process
					// the actual analytics request and prepare the result.
					// In this example, our analytics is so simple. We will do an addition and a multiplication.
					clearM(CR.calcResults);
					appendM(CR.calcResults, CR.operand1 + CR.operand2);
					appendM(CR.calcResults, CR.operand1 * CR.operand2);
					
					// If it is a primary PE, we will submit it.
					// Please note that we are doing a check to see that we have received at least
					// one heart beat reminder. This will prevent a newly restarted former failed primary PE from
					// beginning to process the analytics requests again. 
					if ((_primarySecondaryIdentifier == $PRIMARY_OPERATOR) && (_hbrCnt > 0ul)) {
						submit(CR, Result);
						_lastSequenceNumberProcessedByThePrimary = CR.calcRequestSequenceNumber;
					} else if ((_primarySecondaryIdentifier == $SECONDARY_OPERATOR) && (_missedHeartBeatCnt >= 0)) {
						// If it is a secondary PE, we will simply store the results in a local cache.
						// In case of a current primary PE's failure and when the current
						// secondary PE takes over control and becomes the new primary, we will
						// transmit all the result tuples from the point at which the failure happened.
						//
						// Missed heart beat count for a secondary PE will be less than 0 for a brief period when
						// a failed primary PE is restarted with a new role as a secondary PE. (see the logic for onTuple HBR)
						// During that brief period, we will not cache anything in the results cache as indicated in the "else if" condition above.
						appendM(_analyticsResultTuplesSinceTheLastHeartBeat, CR);
					} 
				}

			config
				// If needed, you can also isolate the primary and secondary by placing them
				// on two different machines. You can add a host section to the following
				// placement configuration with a combination of PERL and SPL.
				placement: partitionColocation("Analytics_Operator_2");
				restartable: true;
				relocatable: true;
		}



	// Let us mow collect the analytic results and write it to a file.
	() as ResultsSink = FileSink(
		 CalcResultFrom1 , CalcResultFrom2  as CR ) {
		logic
			state: {
				mutable uint64 _lastSequenceNumberProcessedByThePrimary = 0ul; 
			}
			
			// If we see any missing or duplicate sequence numbers, we will write about it in the PE console log file.
			onTuple CR: {
				if ((_lastSequenceNumberProcessedByThePrimary == 0ul) ||
					(CR.calcRequestSequenceNumber == _lastSequenceNumberProcessedByThePrimary + 1ul)) {
					// This is the very first result arriving here or the subsequent results arriving in the correct order.
					_lastSequenceNumberProcessedByThePrimary = CR.calcRequestSequenceNumber;
				} else if (_lastSequenceNumberProcessedByThePrimary > CR.calcRequestSequenceNumber) {
					// It looks like a duplicate sequence number. 
					printStringLn("XXXXXXX Detected duplicate sequence number: " + (rstring)CR.calcRequestSequenceNumber);
				} else if ((CR.calcRequestSequenceNumber - _lastSequenceNumberProcessedByThePrimary) > 1ul) {
					// It looks like there are missing tuples in the application flow.
					printStringLn("YYYYYYY Detected missing sequence number(s): " + 
						(rstring)(_lastSequenceNumberProcessedByThePrimary+1ul) + "..." + (rstring)(CR.calcRequestSequenceNumber-1ul));
				}
			}
			
		param
			file: "Calc-Results1.csv";
	}
}

~~~~~~
