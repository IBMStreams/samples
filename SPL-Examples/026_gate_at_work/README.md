~~~~~~
/*
This is an example that uses the Gate operator from the standard toolkit.
This operator delays the incoming tuples until a downstream operator 
signals with an acknowledgment to receive any further tuples. This is 
a great way to have a feedback through which we can control the rate at
which tuples are passed through. (Please refer to another example
named 905_gate_load_balancer that shows the effectiveness of the Gate
operator in combination with the ThreadedSplit operator to provide
load balancing of the incoming tuples.
*/
namespace my.sample;

composite Main {
	type
		ticker = tuple<rstring symbol, float32 price, uint32 quantity, rstring tradeType>;
		ack = tuple<uint32 tuplesAcknowledged>;

	graph
		// Beacon ticker tuples now as fast as possible.
		stream <ticker> BeaconedTicker = Beacon() {
			param
				initDelay: 3.0f;
				iterations: 70000u;
		} // End of Beacon()

		// Let us gate the ticker tuples by coordinating with the
		// acknowledgement received by a downstream operator.
		stream <ticker> Ticker = Gate(BeaconedTicker; Acknowledgement) {
			param
				maxUnackedTupleCount: 1000u;
				numTuplesToAck: Acknowledgement.tuplesAcknowledged;
			
		} // End of Ticker = Gate(BeaconedTicker)
		
		// Let us have a downstream operator that will acknowledge after some delay.
		(stream <ticker> DelayedTicker; stream<ack> Acknowledgement) = Custom(Ticker) {
			logic
				state: {
					mutable uint32 tuplesReceived = 0u;
					mutable uint32 cnt = 0u;
				} // End of state:
				
				onTuple Ticker: {
					// After receiving 950 tuples, we will send an acknowledgement.
					// It is just a random thing here. Typically, application-specific
					// conditions will decide when to send the acknowledgement.
					if (++tuplesReceived == 950u) {
						printStringLn((rstring) ++cnt +  "a) We received 950 tuples and sending an ack to the Gate operator now.");
						tuplesReceived = 0u;
						submit ({tuplesAcknowledged = 950u}, Acknowledgement);
					} // End of if (++tuplesReceived == 950u)
				} // End of onTuple DelayedTicker
				
		} // End of (...) = Custom(Ticker).
					
} // End of composite Main

~~~~~~
