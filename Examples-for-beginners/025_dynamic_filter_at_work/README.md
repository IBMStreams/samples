~~~~~~
/*
This example deals with an intersting standard toolkit operator called
DynamicFilter. This operator is a special version of the Filter operator
that you have already seen in another example; it decides at runtime
which input tuples will be passed through, based on the control input
it receives. This operator is applicable in many real-life scenarios.
This example also demonstrates using a second composite operator to 
perform a sub-task that the main composite will make use of. There is
also coverage to show how the second composite can take its own 
operator parameters.  
*/
namespace my.sample;
// This example demonstrates the use of the Dynamic Filter.
// It is important to note that the DynamicFilter is slightly different from the
// regular Filter operator. DynamicFilter only allows equality based match.
// It doesn't allow < or > or <= or >=.
//
// 
// The DynamicFilter operator below will have three input ports:
// Input Port1: The regular application specific stream will go over this port.
// Input Port2: An outside/external control stream that will carry values for the filter match equality expressions.
// Input Port3: An outside/external control stream that will carry values to be removed from the filter match equality expressions.
// 
// The DynamicFilter operator below will have two output ports.
// Output Port1: All the matching tuples will be sent on this port.
// Output Port2: All the unmatched tuples will be sent on this port.
// 
// 
// This is the way this application works:
// 
// a) Open the data/AddMatchValues.txt and data/RemoveMatchValues.txt files and
// remove if there are any lines below the comment header and save.
// (To begin with, there shouldn't be any line below the comment header.)
// 
// b) Run the application (either in Standalone or in Distributed mode)
//
// c) When the application is started, the Beacon operator will keep generating
// a continuous stream of tuples. But, none of them will be matched initially since
// there are no match values to begin with.
// 
// d) In order to make a match, open the data/AddMatchValues.txt file and 
// add a CSV tuple value there at the last line of the file and save. You can keep
// adding as many lines as you need. If those tuple values match with
// the tuples on the first input port, it will emit an output tuple on its
// first output port which will get displayed on the console screen or the console log file.
// (e-g)
// IBM,buy
// GM,sell
// AMZN,buy
// LLY,sell 
//
// e) In order to stop making a match i.e. filter something that is 
// being matched, enter a tuple value at the last line of the 
// data/RemoveMatchValues.txt file. It will immediately filter any such
// tuples and block them from being sent on the first output port. Instead,
// all the unmatched tuples will be sent out on the second output port. 
// (e-g)
// AMZN,buy
// GM,sell
//
composite Main {
	type
		ticker = tuple<rstring symbol, float32 price, uint32 quantity, rstring tradeType>;
		addKey = tuple<rstring symbol, rstring tradeType>;
		removeKey = addKey;

	graph
		// Beacon ticker tuples now.
		stream <ticker> BeaconedTicker = Beacon() {
			param
				initDelay: 5.0f;
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

		// Let us now read the first external control stream from a hot file.
		// This control stream will bring the values for the eqaulity based filter match.
		stream <addKey> AddKey = FileSource() {
			param
				file: "AddMatchValues.txt";
				format: csv;
				hotFile: true;
		} // End of AddKey = FileSource()	

		// Let us now read the first external control stream from a hot file.
		// This control stream will bring the values for the eqaulity based filter match.
		stream <removeKey> RemoveKey = FileSource() {
			param
				file: "RemoveMatchValues.txt";
				format: csv;
				hotFile: true;
		} // End of RemoveKey = FileSource()	
		
		// Let us do the Dynamic Filtering now.
		(stream <ticker> MatchedTickers; stream <ticker> UnMatchedTickers) = 
			DynamicFilter(Ticker; AddKey; RemoveKey) {
			logic
				onTuple AddKey:
					printStringLn ("AddKey value: " + (rstring) AddKey);	
				
				onTuple RemoveKey: 
					printStringLn ("RemoveKey value: " + (rstring) RemoveKey);
									
			param
				key: Ticker.symbol, Ticker.tradeType;
				addKey: AddKey.symbol, AddKey.tradeType;
				removeKey: RemoveKey.symbol, RemoveKey.tradeType;	
		} // End of (...) = DynamicFilter(Ticker; AddKey; RemoveKey)
	
		// Let us now display the matched streams.
		() as ScreenWriter1 = Writer(MatchedTickers) {
			param
				writerIdentifier: "a";
				matchType: "Matched";
				displayOwner: "Dynamic Filter";
		} // End of ScreenWriter1 = Writer(MatchedTickers)	
		
		// If needed, we can also display the unmatched streams.
		// Since, the Beacon is generating unlimited number of tuples,
		// it may be too much to display/log all the unmatched tuples.
		// Hence, the code block below is commented.
		/*
		() as ScreenWriter2 = Writer(UnMatchedTickers) {
			param
				writerIdentifier: "b";
				matchType: "UnMatched";
		} // End of ScreenWriter2 = Writer(UnMatchedTickers)
		*/				
} // End of the main composite.

// This composite will write status messages either to the screen or
// to the console log file depending on how the application is run.
// i.e. Standalone mode or Distributed mode.
public composite Writer(input I) {
	param
		expression <rstring> $writerIdentifier;
		expression <rstring> $matchType;
		expression <rstring> $displayOwner;

	graph
		() as ScreenWriter1 = Custom(I) {	
			logic
				state: 
					mutable int32 cnt = 0;
				
				onTuple I: {
					if (cnt++ == 0) {
						printStringLn("\n" + $writerIdentifier + ") " + $matchType + " tuples from the " + $displayOwner + ":");
					} // End of if (cnt++ == 0)
					
					printStringLn ((rstring) cnt + $writerIdentifier + ") " + (rstring) I);
				} // End of onTuple I
		} // End of ScreenWriter1 = Custom(I)
		
} // End of composite Writer(input I)

~~~~~~
