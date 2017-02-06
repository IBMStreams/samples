~~~~~~ Scala 
namespace word.count;

type LineStat = tuple<int32 lines, int32 words>;

int32 countWords(rstring line) {
	return size(tokenize(line, " \t", false));
	
} // End of function countWords

void addStat(mutable LineStat x, LineStat y) {
	x.lines += y.lines;
	x.words += y.words;
} // End of function addStat

~~~~~~

---
~~~~~~ Scala 
/*
This example is the same code that can be found in the SPL introductory tutorial PDF file.
Please see that PDF file for a description about what this application does.
*/
namespace word.count;

composite WordCount {
	graph
		stream <rstring lineContents> Data = FileSource() {
			param
				format: line;
				file: getSubmissionTimeValue("file");
				// file		: "catFood.txt";
		} // End of FileSource.
		
		stream <LineStat> OneLine = Functor(Data) {
			output
				OneLine: 
					lines = 1, words = countWords(lineContents);
		} // End of Functor.
		
		() as Counter = Custom(OneLine) {
			logic
				state: mutable LineStat sum = {lines = 0, words =0};
				
				onTuple OneLine: 
					addStat (sum, OneLine);

				onPunct OneLine: {
					if (currentPunct() == Sys.FinalMarker) {println(sum);}
				} // End of onPunct OneLine
		} // End of Custom.
} // End of WordCount.

~~~~~~
