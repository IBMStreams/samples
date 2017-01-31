~~~~~~
/*
This example shows the use of multiple composites in a single application. 
There is a main composite that in turn uses two other composites. This application
shows how the additional composites in different namespaces get included into
the main composite via the "use" directive. It also demonstrates how the additional
composites can accept their own operator parameters. It teaches the basics of an 
important feature that will come handy when big applications need to be componentized. 
*/
namespace my.sample1;
use my.sample2::StockMatch;
use my.sample3::StockOrderCommission;

composite Main {
	type
		stockOrder = tuple<rstring symbol, float32 price, uint32 quantity, rstring tradeType, boolean matchFound, float32 commission>;
	
	graph
		stream <stockOrder> StockOrderRecord = FileSource() {
			param
				file: "StockOrders.txt";
				format: csv;
				initDelay: 3.0f;
				hasDelayField: true;
		} // End of StockOrderRecord = FileSource()
		
		// Call a composite to decide if this stock order has a match.
		stream <stockOrder> MatchStockOrderResult = StockMatch(StockOrderRecord) {
			param
				matchTradeType: "buy";
				minimumMatchQuantity: 100u;
				minimumMatchPrice: (float32)50.45f;
		} // End of MatchStockOrderResult = StockMatch(StockOrderRecord)
		
		// Call a composite to compute the commission fee.
		stream <stockOrder> MatchedStockOrder = StockOrderCommission(MatchStockOrderResult) {			
		} // End of MatchedStockOrder = StockOrderCommission(MatchStockOrderResult)
		
		// Display the resulting order matched tuples on the console.
		() as ScreenWriter1 = Custom(MatchedStockOrder) {
			logic
				state:
					mutable uint32 cnt = 0u;
			
				onTuple MatchedStockOrder: {
					if (cnt++ == 0u) {
						printStringLn("\na) Matched stock orders:");
					} // End of if (cnt++ == 0)
					
					printStringLn ((rstring) cnt + "a) " + (rstring) MatchedStockOrder);
				} // End of onTuple MatchedStockOrder
		} // End of ScreenWriter1 = Custom(MatchedStockOrder)
} // End of composite Main

~~~~~~

---
~~~~~~
namespace my.sample2;

public composite StockMatch(output Output; input Input) {		
	param
		expression <rstring> $matchTradeType;
		expression <uint32> $minimumMatchQuantity;
		expression <float32> $minimumMatchPrice;

	type
		stockOrder = tuple<rstring symbol, float32 price, uint32 quantity, rstring tradeType, boolean matchFound, float32 commission>;
	
	graph
		stream <stockOrder> ResultFromStockMatch = Functor(Input) {
			param
				filter: (symbol == "IBM" || symbol == "AAPL") &&
					   (tradeType == $matchTradeType) &&
					   (quantity >= $minimumMatchQuantity) &&
					   (price >= $minimumMatchPrice);
			
			output
				ResultFromStockMatch: matchFound = true;
		} // End of ResultFromStockMatch = Functor(Input)
	
		// Send this tuple out now.
		stream <stockOrder> Output = Custom(ResultFromStockMatch) {
			logic
				onTuple ResultFromStockMatch:
					submit(ResultFromStockMatch, Output);
		} // End of Output = Custom(ResultFromStockMatch)
} // End of StockMatch(output Output; input Input)

~~~~~~

---
~~~~~~
namespace my.sample3;

public composite StockOrderCommission (output Output; input Input) {
	type
		stockOrder = tuple<rstring symbol, float32 price, uint32 quantity, rstring tradeType, boolean matchFound, float32 commission>;

	graph
		// All we need to here is to create a random commission fee
		// for the matched orders sent to us.
		stream <stockOrder> Output = Custom(Input) {
			logic
				state:
					mutable tuple <stockOrder> myOutput = {};

				onTuple Input: {										
					myOutput.symbol = Input.symbol;
					myOutput.price = Input.price;
					myOutput.quantity = Input.quantity;
					myOutput.tradeType = Input.tradeType;
					myOutput.matchFound = Input.matchFound;
					myOutput.commission = (float32) random()* (float32)10.0f;
					
					// Send it away.
					submit(myOutput, Output);
				} // End of OnTuple Input:
		} // End of Output = Custom(Input)
} // End of composite StockOrderCommission


~~~~~~
