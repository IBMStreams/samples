~~~~~~
/*
This example shows a cool SPL feature called mixed-mode support. In this,
developers can mix PERL code islands inside of an SPL application.
Mixed-mode enables the easy parameterization of SPL applications. This 
example gives a small flavor of how a PERL code snippet inter-mixed
with SPL allows us to parameterize the SPL Stream names and the
number of output stream definitions for an SPL operator. 
*/
namespace my.sample;

composite Main {
	type StockReportSchema = tuple <rstring symbol, rstring dateTime, float64 closingPrice, uint32 volume>;

	graph
		stream<StockReportSchema> StockReport = FileSource() {
			param
				file: "stock_report.dat";
				format: csv;
				hasDelayField: true;
		} // End of FileSource.

		
		(
			stream<StockReportSchema> StockReportOutput1
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput2
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput3
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput4
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput5
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput6
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput7
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput8
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput9
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput10
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput11
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput12
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput13
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput14
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput15
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput16
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput17
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput18
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput19
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput20
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput21
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput22
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput23
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput24
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput25
 
			
				;
			
		
			stream<StockReportSchema> StockReportOutput26
 
			
		) 
		= Split(StockReport) {
			param
			// index: hashCode(toCharacterCode(symbol, 0) - toCharacterCode("A", 0));
			file: "mapping.txt";
			key:  symbol;
		} // End of Split

		() as FileWriter1 = FileSink(StockReportOutput1) {
			param file: "split_ticker_output_stream_1.result";
		} // End of FileSink(StockReportOutput1)

		() as FileWriter2 = FileSink(StockReportOutput7) {
			param file: "split_ticker_output_stream_7.result";
		} // End of FileSink(StockReportOutput7)

		() as FileWriter3 = FileSink(StockReportOutput9) {
			param file: "split_ticker_output_stream_9.result";
		} // End of FileSink(StockReportOutput9)
} // End of composite Main

~~~~~~
