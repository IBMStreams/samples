~~~~~~
/*
This example shows how to create source operators using the Custom operator available in
the SPL standard toolkit. Starting in Streams 3.x, it is possible to create source operators
without writing primitive source operators in C++ or Java. Simple source operators can be
written using the built-in SPL Custom operator. This will come handy for those who don't
want to do an extra layer of C++ or Java code for satisfying simple needs for a source operator.
You will see a function of a file source operator being implemented all using SPL code in this example.
*/
namespace my.file.source;

use spl.file::*;

composite file_source_using_spl_custom_operator {
	param
		// This example requires the user to pass a command line argument to this application.
		// While running the standalone code, simply add the following to your application launch command.
		// my.file.source::file_source_using_spl_custom_operator.SourceFileName=<Substitute here the file name you want to read from>
		// In this particular example, you can give the following file which is already available
		// in this application's data directory.
		// my.file.source::file_source_using_spl_custom_operator.SourceFileName=stock_report.dat
		expression<rstring> $sourceFileName : getSubmissionTimeValue("SourceFileName");
			
	type
		StockReportSchema = tuple <rstring symbol, 
			rstring dateTime, float64 closingPrice, uint32 volume>;

	graph
		// Define a file source functionality using the SPL Custom operator.
		// You can see below that this Custom operator doesn't take any input streams as that
		// is true for any soure operator.
		stream<StockReportSchema> StockRecord = Custom() {
			logic
				state: {
					mutable uint32 _tupleCnt = 0u;
					mutable StockRecord _stockReport = {};
				}
				
				// When a Custom operator does the source function, then we are allowed
				// to have an onProcess clause. We can simply be in a loop here until
				// we ingest all the records from our data source. (In this case our
				// data source is a file specified by the user at the application launch time.)
				onProcess: {
					// Open the user-specified file.
					mutable uint64 _fileHandle = 0ul;
					mutable int32 _errorCode = 0;
					// In Streams 4.x, data directory that is part of the application directory must be manually
					// obtained via an SPL API as shown below.
					_fileHandle = fopen (dataDirectory() + "/" + $sourceFileName, "r", _errorCode);
					
					// Handle file open errors.
					if (_errorCode != 0) {
						appTrc(Trace.error, "Unable to open file " + dataDirectory() + "/" + $sourceFileName +
							", error=" + (rstring)_errorCode);
						abort();
					}
					
					// Stay in a loop and keep ingesting the data from the given file.
					while(true) {
						rstring _line = freadLine (_fileHandle, 
							_errorCode);

						if (_errorCode == -1) {
							// EOF check
							submit(Sys.FinalMarker, StockRecord);
							fclose(_fileHandle, _errorCode);
							break;
						}

						// Handle file read errors.
						if (_errorCode != 0) {
							appTrc(Trace.error, "Error reading from file " +
								$sourceFileName +
								", error=" + (rstring)_errorCode);
							abort();
						}
						
						// Split the CSV record/row read from the file into string tokens.
						list<rstring> _tokens =  csvTokenize (_line);
						
						// Initialize empty values for a new tuple.
						_stockReport = (StockRecord){};
						// Assign the tuple attributes as read from the file.
						_stockReport.symbol = _tokens[1];
						_stockReport.dateTime = _tokens[2];
						// Do appropriate type casting from the string type.
						_stockReport.closingPrice = (float64)_tokens[3];
						_stockReport.volume = (uint32)_tokens[4];
						// Send this tuple away.
						submit(_stockReport, StockRecord);
						_tupleCnt++;
					} // End of while loop
				}
		}
		
		// Write the tuples received from the source input file to a sink output file.
		() as FileSink1 = FileSink(StockRecord) {
			param
				file: "stock_report_output.txt";
				format: csv;
				flush: 2u;
				// Don't quote the strings in the output CSV columns.
				quoteStrings: false;
		}

}

~~~~~~
