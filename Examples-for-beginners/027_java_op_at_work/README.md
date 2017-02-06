~~~~~~ Scala 
/*
This example shows an important operator that brings Java into the C++ dominated
world of Streams!!! That operator is called JavaOp, which is used to call out to
other operators implemented in Java using the Java Operator API. In this example,
we will have a tiny Java logic that will calculate the current time and add that
time string to a tuple attribute and output that tuple. There is another example
that shows the Java primitve operator that is different from the JavaOp operator.
[JavaOp operator was added to Streams much before Streams started supporting a
 framework for Java primitive operators. Since Java primitive operators have become 
 commonplace in Streams applications starting in 2013, there is rarely a reason to
 use the JavaOp operator. In case if it is required, JavaOp is still there.]
*/
namespace my.sample;
// This application shows a simple Java operator.
composite Main {
	type
		ticker = tuple<rstring symbol, float32 price, uint32 quantity, rstring tradeType, rstring dateTime>;

	graph
		// Beacon ticker tuples now.
		stream <ticker> BeaconedTicker = Beacon() {
			param
				initDelay: 3.0f;
				iterations: 500u;
		} // End of Beacon()
		
		// Enrich the Beacon generated tuple now.
		stream <ticker> Ticker = Custom(BeaconedTicker) {
			logic
				state: {
					mutable uint32 cnt = 0;
					list<rstring> symbols = ["GM", "PFE", "F", "LLY", "BMS", "IBM", "GOOG", "AMAT", "GE", "AMZN"];
					mutable list<float32> prices = [170.34f, 23.12f, 620.34f, 54.67f, 68.34f, 205.15f, 650.56f, 58.34f, 73.12f, 190.67f];
					mutable list<uint32> quantities = [156u, 215u, 100u, 165u, 178u, 560u, 345u, 410u, 200u, 325u];
					list<rstring> tradeTypes = ["buy", "sell"];
					mutable uint32 tradeTypeIndicator = 0; 
					mutable tuple<ticker> result = {};  	
				} // End of state:
			
			onTuple BeaconedTicker: { 
				cnt = ((uint32) (random()*100.0))%10u;
				
				// Alternate between "buy" and "sell".
				if (tradeTypeIndicator == 0u) {
					tradeTypeIndicator = 1u;
				} else {
					tradeTypeIndicator = 0u;
				}
				
			    result.symbol = symbols[cnt];
				result.price = prices[cnt];
				result.quantity = quantities[cnt];
				result.tradeType = tradeTypes[tradeTypeIndicator];
				//Send this tuple away.
				submit(result, Ticker);
			} // End of BeaconedTicker:
		} // End of Ticker = Custom(BeaconedTicker)	
		
		// Let us call a simple (generated) JavaOp now.
		// We will begin this composite with just the following 
		// code snippet and without any Java classes.
		// The very first time this SPL file gets compiled,
		// the SPL compiler will create a skeleton of the
		// Java source code for the class specified in the className parameter below.
		// That source file is found in your application directory under
		// "opt/java/src" sub-directory.
		// Corresponding binary (byte code) for that class is found in
		// in the "output/Your_NameSpace/Standalone/java/bin" directory.
		stream <ticker> TickerWithTime = JavaOp(Ticker) {
			param
				generated: true;
				className: "com.ibm.test.AddTime";
				classLibrary: "bin";
		} // End of TickerWithTime = JavaOp(Ticker)
		
		// Let us display the output from the Java Operator.
		stream <ticker> TickerResult = Functor(TickerWithTime) {
			logic
				onTuple TickerWithTime:
					printStringLn((rstring) TickerWithTime);
		} // End of TickerResult = Functor(TickerWithTime)
} // End of composite Main
~~~~~~
