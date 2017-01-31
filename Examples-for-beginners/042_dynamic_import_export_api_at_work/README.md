~~~~~~
/*
This example shows how to use the SPL APIs for dynamically importing and exporting streams.
This is achieved by changing the import and export properties on the fly. This powerful
feature in Streams provides a way to change the streams producing and consuming operators
to change the way in which they publish and subscribe to streams while the application is running.

The main composite below dynamically changes the exported stream property value after every minute.
In order to test this, always launch the dynamic_export composite first in distributed mode.
After that, launch the dynamic_import composite in distributed mode.  You can watch this whole
thing working inside the Streams Instance Graph by starting it from the Streams Explorer view.
*/
namespace dynamic.importing.exporting;

composite dynamic_export {
	type
		SimpleTransaction = tuple<int32 number, rstring string>;
	
	graph
		// Create a Beacon signal every 10 seconds.
		stream<SimpleTransaction> SimpleTxStream = Beacon() {
			param
				iterations: 7000;
				period: 10.0;
		} // End of SimpleTxStream = Beacon()


		// Receive the Beacon signal and customize the tuple attributes and send it for export.
		stream<SimpleTransaction> ModifiedSimpleTxStream = Custom(SimpleTxStream as STS) {
			logic
				state: {
					mutable int32 _tupleCnt = 0;
					mutable int32 _secondsElapsedSinceExportPropertyWasChanged = 0;
				} // End of state
				
				onTuple STS: {
					// Modify the tuple attributes and send it away.
					STS.number = ++_tupleCnt;
					STS.string = "String " + (rstring)_tupleCnt;
					// Before submitting this tuple, let us change the exported property value using 
					// the new API available in Streams 3.0 and later releases. 
					// This is an arbitrary change just to showcase a new SPL API.
					// It demonstrates how output port export properties can be dynamically changed.
					// Let us do this dynamic change periodically.
					// After every minute, we will dynamically change the exported stream property value by 
					// changing the property value to the current value of one of the attributes of the exported stream.
					// Beacon triggers every 10 seconds. Hence, we will advance the elapsed time by 10 seconds.
					_secondsElapsedSinceExportPropertyWasChanged += 10;
					
					if (_secondsElapsedSinceExportPropertyWasChanged >= 60) {
						// It is time to a change the exported stream property value.
						// Reset the timer flag to 0 to wait for the next minute of time to pass by.
						_secondsElapsedSinceExportPropertyWasChanged = 0;
						// We will change the exported property value by setting it to the current value of the "number" tuple attribute.
						int32 rc = setOutputPortExportProperties({AllowedMinimumNumber=(int64)STS.number}, 0u);
					
						if (rc != 0) {
							// Log the error and abort the application.
							log(Sys.error, "SetOutputPortExportProperties API failed with return code " + (rstring)rc);
							abort();
						} // End of if (rc != 0)
					} // End of if (_secondsElapsedSinceExportPropertyWasChanged >= 60)
					
					// Send it on the first output port index (New submit API in Streams 3.0)
					submit(STS, 0u);
				} // End of onTuple STS
		} // End of ModifiedSimpleTxStream = Custom(SimpleTxStream as STS)
		
		// Export the modified simple Tx stream.
		() as ExportedSimpleTxStream = Export(ModifiedSimpleTxStream as MSTS) {
			param
				// To begin with, set the exported stream property value to a default of 0.
				// This property value will be dynamically changed after every minute using the new SPL API.
				properties: {AllowedMinimumNumber = 0l};
		} // End of ExportedSimpleTxStream = Export(ModifiedSimpleTxStream)		 
} // End of composite dynamic_export
~~~~~~

---
~~~~~~
/*
This example shows how to use the SPL APIs for dynamically importing and exporting streams.
This is achieved by changing the import and export properties on the fly. This powerful
feature in Streams provides a way to change the streams producing and consuming operators
to change the way in which they publish and subscribe to streams while the application is running.

This main composite dynamically changes the imported stream subscription value after every minute.
In order to test this, always launch the dynamic_export composite first in distributed mode.
After that, launch the dynamic_import composite in distributed mode. You can watch this whole
thing working inside the Streams Instance Graph by starting it from the Streams Explorer view.
*/
namespace dynamic.importing.exporting;

composite dynamic_import {
	type
		SimpleTransaction = tuple<int32 number, rstring string>;
	
	graph
		// Import the stream emitted by the dynamic_export main composite.
		stream<SimpleTransaction> ImportedSimpleTxStream = Import() {
			param
				// To begin with, set an import subscription that will not find a match with the exported stream from another composite.
				subscription: AllowedMinimumNumber < 0l;
		} // End of ImportedSimpleTxStream = Import()
		
		// Let us have a Beacon to periodically send a signal to change the input port import subscription.
		stream<int32 subscriptionChangeSignal> ImportSubscriptionChangeSignal = Beacon() {
			param
				period: 5.0;
		} // End of ImportSubscriptionChangeSignal = Beacon()
		
		// Let us change the import subscription value in this Custom operator.
		stream<SimpleTransaction> SimpleTxStreamForOutput = Custom(ImportSubscriptionChangeSignal as ISCS; 
			ImportedSimpleTxStream as ISTS) {
			logic
				state: {
					mutable int32 _secondsElapsedSinceImportSubscriptionWasChanged  = 0l;
					mutable boolean _matchExportedStreamProperty = false;
					mutable rstring _newSubscription = "";
				} // End of state
				
				onTuple ISCS: {
					// The other main composite in this application (dynamic_export) will keep sending
					// one exported tuple every 10 seconds. After every minute of time passes by, that application also
					// changes its exported stream property to the value of one of its tuple attributes (int32 number).
					// It does that just to show how an output port export property can be 
					// dynamically changed on the fly.
					//
					//
					// Similarly, in the current main composite, where you are now (dynamic_import), we will
					// write some code below to demonstrate how import subscription can also be 
					// dynamically changed using a new API available in Streams 3.0.
					// In order to do that, after every 30 seconds, we will arbitrarily change the
					// import subscription so as to toggle the matching of the exported stream property.
					// i.e. for one 30 second block, we will not match any exported stream property and
					// in the next 30 second block, we will match with the exported stream property.
					// We are doing this logic just to show the reader of this application, how both
					// the export property and import subscription can be changed dynamically.
					_secondsElapsedSinceImportSubscriptionWasChanged += 5;					
					
					if (_secondsElapsedSinceImportSubscriptionWasChanged >= 30) {
						// It is time to a change the import subscription value.
						// Reset the timer flag to 0 to wait for the next 30 seconds of time to pass by.
						_secondsElapsedSinceImportSubscriptionWasChanged = 0;
						
						// On every import subscription change event, toggle either to match or not match with the exported stream property.
						if (_matchExportedStreamProperty == false) {
							// Set the import subscription such that we will match the exported stream property.
							_matchExportedStreamProperty = true;
							_newSubscription = "AllowedMinimumNumber >= 0"; 
						} else {
							// Set the import subscription such that we will not match the exported stream property.
							// Please note that, we are doing this arbitrary logic just to showcase
							// SPL subscription API.
							_matchExportedStreamProperty = false;
							_newSubscription = "AllowedMinimumNumber < 0"; 
						} // End of if (_matchExportedStreamProperty == false)
					
						// Let us keep changing the import subscription value periodically.
						int32 rc = setInputPortImportSubscription(_newSubscription, 1u);
	
						if (rc != 0) {
							// Log the error and abort the application.
							log(Sys.error, "New Subscription=" + _newSubscription);
							log(Sys.error, "SetInputPortImportSubscription API failed with return code " + (rstring)rc);
							abort();
						} // End of if (rc != 0)
					} // End of if (_secondsElapsedSinceImportSubscriptionWasChanged >= 30)
				} // End of onTuple ISCS
				
				onTuple ISTS: {
					// Send it on the first output port index (New submit API in Streams 3.0)
					submit(ISTS, 0u);				 
				} // End of onTuple ISTS
		} // End of SimpleTxStreamForOutput = Custom(ImportedSimpleTxStream as ISTS)
		
		() as Sink1 = Custom(SimpleTxStreamForOutput as STSFO) {
			logic
				onTuple STSFO: {
					printStringLn("SimpleTxStream = " + (rstring) STSFO);
				} // End of onTuple STSFO
		} // End of Sink1 = Custom(SimpleTxStreamForOutput as STSFO)
} // End of composite dynamic_import


~~~~~~
