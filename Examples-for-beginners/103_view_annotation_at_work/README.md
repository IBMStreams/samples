~~~~~~
/*
==================================================================================
This is a simple application that explains the steps required to use the
view annotation and then how to visualize the view annotated stream in the
Streams web console.

Please see the detailed steps written above the Custom operator. 
==================================================================================
*/
namespace com.acme.test;

composite ViewAnnotationAtWork {
	graph
		stream<int32 x, int32 y> TestData = Beacon() {
			param
				period: 0.100;
		}
		
		// Declare a view annotation so that this can be viewed either in Excel or
		// in the Streams web console.
		// We describe here the simple steps required to visualize this inside the Streams Web Console:
		//
		// 1)  Run this application in distributed mode.
		// 2)  Start the web console by typing this command in an xterm window:
		//     streamtool   launch    --console
		//     (OR)
		//     Start a web browser and give the URL for Streams web console.
		//     You can get the URL this way:   streamtool   geturl  -d <YOUR_DOMAIN_NAME>
		// 3)  Streams web console will prompt for your userid and password. Enter them to login.
		// 4)  In the web console, click on the STREAMS tab at the very top.
		// 5)  It will display two streams from this application.
		// 6)  Select the PlotData[0]-->MySink1[0] and then click "Monitor Stream".
		// 7)  You will immediately see "View Annotation" card displaying that streaming data in tabular form.
		// 8)  You can now create a Dynamic chart: Click the wheel icon (Card Settings) located at the bottom left of the screen that says "Dynamic Chart".
		// 9)  Click at the "Create Dynamic Chart" option. In the Chart tab, select your chart style (e-g: Clustered Bar) and the
		//     titles for your X & Y Axis. Click OK.
		// 10) You should see your dynamic chart displayed in real time for the PlotData which has the view annotation.
		@view(name="PlotView", port=PlotData, sampleSize=10, bufferSize=20)
		stream<TestData> PlotData = Custom(TestData as TD) {
			logic
				state: {
					mutable int32 _x = 0;
					mutable PlotData oTuple = {};
				}
				
				onTuple TD: {
					// Stepwise increase for x.
					oTuple.x = ++_x;
					// Random increase for y.
					float64 randValue = random();
					oTuple.y = (int32)(randValue * 1000.00);		
					submit(oTuple, PlotData);
				}
		}
		
		() as MySink1 = FileSink(PlotData) {
			param
				file: "/dev/null";
		}
}

~~~~~~
