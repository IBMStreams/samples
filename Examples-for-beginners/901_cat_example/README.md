~~~~~~
/*
This example is the same code that can be found in the SPL introductory tutorial PDF file.
Please see that PDF file for a description about what this application does.
*/
composite NumberedCat {
	graph
		stream <rstring contents> Lines = FileSource() {
			param
				format: line;
				file: getSubmissionTimeValue("file");
					//file			: "catFood.txt";	
		} // End of FileSource.
		
		stream <rstring contents> Numbered = Functor(Lines) {
			logic
				state: {
					mutable int32 i = 0;
				}
				
				onTuple Lines: { 
					i++;
				}
					
			output
				Numbered: contents = (rstring)i + " " + Lines.contents;
		} // End of Functor.
		
		() as Sink = FileSink(Numbered) {
			param
				file: "result.txt";
		} // End of Sink.
} // End of NumberedCat.

~~~~~~
