/*
This is a companion Java project for an SPL project (053_java_primitive_operator_with_complex_output_tuple_types)
present in the same Eclipse workspace. This Java class extends from a Streams AbstractOperator
class. It receives an input tuple with a few dummy attributes (int32, string, and list<int32>). 
Then, this Java primitive operator creates an output tuple with a list of complex typed  attributes.
It then submits that tuple.

This example Java primitive operator and the SPL file (in 053_XXXXX project) together answer
the following questions.

1) How can we ingest an input tuple with attributes of different types and access those
   tuple attributes inside a Java operator?

2) How can we write to the PE trace file from a Java operator?

3) How can we write to the PE log file from a Java operator?

4) How can we write to the PE console output file from a Java operator?

5) More importantly, how can we create an output tuple containing a list of 
   complex typed attributes and then submit it inside a Java operator?

Since the Java class from this Java project forms the basis for the Java primitive operator 
of the above-mentioned SPL project, the compiled Java class file from this Java project 
should be manually copied to the impl/java/bin directory in the SPL project. 
Under that bin directory, you are required create the necessary sub-directories
that accurately reflect the package name of this java class.
Please read more detailed comments documented in the Main.spl file in the SPL project.

 IMPORTANT:
 This Java class is extended from a Streams AbstractOperator and in addition it uses
 a lot of other Streams Java artifacts such as the Tuple class. In order to resolve those
 Streams Java classes and compile it correctly, you have to do the following.
 
 a) Ensure you are in the Eclipse Java perspective.
 b) Right click on the Java_Complex_Type_Submission and select Properties.
 c) In the left pane of the resulting dialog, click on "Java Build Path".
 d) In the right pane, click on the tab titled "Libraries".
 e) Click the "Add External Jars" button, add the following Streams jar files from you
    Streams installation directory.
       
    i)   <Your Streams Install Directory>/lib/com.ibm.streams.operator.jar
    ii)  <Your Streams Install Directory>/lib/com.ibm.streams.operator.samples.jar
                
f) After adding these three dependent jar files, you may delete/remove the three
   erroneous jar entries in your "Libraries" tab that point to invalid directories.
*/
package com.acme.complex.tuple.type.submission;

import com.ibm.streams.operator.AbstractOperator;
import com.ibm.streams.operator.OperatorContext;
import com.ibm.streams.operator.OutputTuple;
import com.ibm.streams.operator.StreamingInput;
import com.ibm.streams.operator.StreamingOutput;
import com.ibm.streams.operator.Tuple;
import com.ibm.streams.operator.meta.*;
import com.ibm.streams.operator.StreamSchema; 
import com.ibm.streams.operator.logging.LogLevel;
import com.ibm.streams.operator.logging.LoggerNames;
import com.ibm.streams.operator.logging.TraceLevel;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.logging.Logger;

// This class must be first compiled without any error in a 
// separate Eclipse Java project named Java_Complex_Type_Submission.
public class ComplexTupleTypeSubmission extends AbstractOperator {
	// Get the class name of this Java primitive operator.
	private static final String CLASS_NAME = ComplexTupleTypeSubmission.class.getName();
        
	/*
	 Create a {@code Logger} specific to this primitive operator class that will write
	 to the SPL trace facility as a child of the root {@code Logger}.
    */
	private final Logger trace = Logger.getLogger(CLASS_NAME);
        
	/*
	Create a {@code Logger} specific to this class that will write
	to the SPL log facility as a child of the {@link LoggerNames#LOG_FACILITY}
	{@code Logger}.
	The {@code Logger} uses a resource bundle.
	*/
	private final Logger log =
		Logger.getLogger(LoggerNames.LOG_FACILITY + "." + CLASS_NAME,
			"com.ibm.streams.operator.samples.operators.messages");

	// Declare an application-specific instance variable.
	public int tupleCnt;
	
	@Override
	public synchronized void initialize(OperatorContext context) throws Exception {
		super.initialize(context);

		// Note setLoggerAspects() must be called after super.initialize(context)
		// as it requires the context.
		        
		// Associate the aspect exampleTrace with trace
		// messages from the SPL trace logger
		setLoggerAspects(trace.getName(), "ExampleTrace");        
		// Associate the aspect exampleLog with messages from the SPL log logger.
		setLoggerAspects(log.getName(), "ExampleLog");
		// Initialize the application-specific instance variable to 0.
		tupleCnt = 0;
	} // End of the method initialize.
	

	// In this process method, we are going to show how to create a complex 
	// output tuple and submit it to the downstream operator.
	public void process(StreamingInput<Tuple> inputStream, Tuple tuple) {
		tupleCnt++;
		// Our input tuple in this example will have a dummy tuple just to show how 
		// a Java primitive operator can deal with incoming tuple attributes.
		// TestInputData_t = int32 dummyInt, rstring dummyString, list<int32> dummyIntList;
		//
		// First, let us show in this example how to access the input tuple attributes.
		// It is so simple.
		// You can pull the input tuple attribute value using the tuple attribute name.
		int _dummyInt = tuple.getInt("dummyInt");
		String _dummyString = tuple.getString("dummyString");
		// To avoid a Java compiler waring, you can use List<?> instead of List<Integer>
		List<?> _dummyIntList = (List<?>) tuple.getList("dummyIntList");
		// When you access the list, you can do the correct casting to an integer.
		int listElement1 = (Integer) _dummyIntList.get(0);
		int listElement2 = (Integer) _dummyIntList.get(1);
		
		// Starting from Streams 3.x, these are the log files that will get created for every PE.
		//
		// There are 4 kinds of log files created for every PE as shown below:
		// They can be found in this directory: /tmp/streams.<YOUR_INSTANCE_NAME>@<YOUR_USER_ID>/jobs/<YOUR_JOB_ID> 
		//
		// peXXX.out            --> Application trace  (Written using the appTrc API)
		// pecXXX.out           --> PEC runtime logs (Daemon startup/Shutdown)
		// pec.peXXX.log        --> Applicaton and PEC logging  (Written using the appLog API)
		// pec.peXXX.stdouterr  --> Any stdout/stderr messages
		
		// Let us now show how to tracing and logging into the PE trace file and 
		// the PE log file using the Java APIs.
		// Trace with a simple message.
		trace.log(TraceLevel.ERROR, "Inside the process method of the ComplexTypeTupleSubmission Java class");
		// Trace with a message containing parameters.
		trace.log(TraceLevel.ERROR, "Input tuple attributes: dummyInt={0}, dummyString={1}, dummyIntList={2}",
			new Object[] {_dummyInt, _dummyString, _dummyIntList});
		
		// Log with a simple message.
		log.log(LogLevel.ERROR, "Inside the process method of the ComplexTypeTupleSubmission Java class");
		// Log with a message containing parameters.
		log.log(LogLevel.ERROR, "Input tuple attributes: dummyInt={0}, dummyString={1}, dummyIntList={2}",
			new Object[] {_dummyInt, _dummyString, _dummyIntList});
		// Log with a message containing an exception object.
		try {
			dummyMethodToThrowAnException();
		} catch (Exception ex) {
			log.log(LogLevel.ERROR, "Don't panic. We are just showing an example of how to log a dummy exception.", ex);
		}
		
		// Write it to the PE console log file.
		System.out.println("Input tuple's dummyIntList has its first element as " + listElement1 +
			" and its second element as " + listElement2 + ".");
		
		// In this example, we are mainly demonstrating how to form a complex tuple and 
		// submit from this Java operator.
		// Following is the output tuple format we will be generating.
		// Output SPL schema is shown below.
		//
		// type SnapshotData_t = ustring ric, map<ustring, float64> dblMap, map<ustring, ustring> strMap;
		// type SnapshotResponse_t = list<SnapshotData_t> responses;
		//		
		// Get an output stream object suitable for output port 0.
		StreamingOutput<OutputTuple> output = getOutput(0);
		// Get the type of the "responses" output tuple attribute and cast it to CollectionType.
		CollectionType ct = (CollectionType) output.getStreamSchema().getAttribute("responses").getType();
		// Get the concrete element type of the collection and cast it to a tuple type.
		TupleType tt = (TupleType) ct.getElementType(); 
		// This collection type holds the SnapShotData_t tuple. Get the schema for that.
		StreamSchema ssd = tt.getTupleSchema();
		
		// Our output tuple contains on;y one attribute: a list of Snapshot Data tuples.
		// Let us create an empty list now to hold a few Snapshot data tuples.
		List<Tuple> _responses = new ArrayList<Tuple>();
		
		// Form the individual attribute values that we need for the SnapshotData tuple.
		java.lang.String _ric1 = new java.lang.String("ric") + Integer.toString(tupleCnt) + "_1";
		Map<String, Double> _dblMap1 = new HashMap<String, Double>();
		_dblMap1.put("IBM", new Double(207.35));
		_dblMap1.put("GOOG", new Double(635.26));
		_dblMap1.put("CSCO", new Double(19.86));
		
		Map<String, String> _strMap1 = new HashMap<String, String>();
		_strMap1.put("IBM", "IT Giant");
		_strMap1.put("GOOG", "Search Giant");
		_strMap1.put("CSCO", "Networking Giant");
		
		// Now create an attribute map for the SnapShotData tuple.
		Map<String, Object> attrMap1 = new HashMap<String, Object>();
		attrMap1.put("ric", _ric1);
		attrMap1.put("dblMap", _dblMap1);
		attrMap1.put("strMap", _strMap1);
		// Using the attribute map created above, create a few SnapshotData tuples.
		Tuple ssdTuple1 = ssd.getTuple(attrMap1);
		// Add the first SnapshotData tuple into our responses list.
		_responses.add(ssdTuple1);
		
		// Create one more SnapshotData tuple.
		java.lang.String _ric2 = new java.lang.String("ric") + Integer.toString(tupleCnt) + "_2";
		Map<String, Double> _dblMap2 = new HashMap<String, Double>();
		_dblMap2.put("HD", new Double(76.34));
		_dblMap2.put("GM", new Double(33.35));
		_dblMap2.put("WMT", new Double(76.28));
		
		Map<String, String> _strMap2 = new HashMap<String, String>();
		_strMap2.put("HD", "Home Goods Giant");
		_strMap2.put("GM", "Auto Giant");
		_strMap2.put("WMT", "Retail Giant");
		
		// Now create an attribute map for the SnapShotData tuple.
		Map<String, Object> attrMap2 = new HashMap<String, Object>();
		attrMap2.put("ric", _ric2);
		attrMap2.put("dblMap", _dblMap2);
		attrMap2.put("strMap", _strMap2);
		// Using the attribute map created above, create a few SnapshotData tuples.
		Tuple ssdTuple2 = ssd.getTuple(attrMap2);
		// Add the second SnapshotData tuple into our responses list.
		_responses.add(ssdTuple2);	
		
		// Create a new output tuple to send out on output port 0.
		OutputTuple outputTuple = output.newTuple();
		// Now set the _responses list to this tuple as its only attribute.
		outputTuple.setList("responses", _responses);

		// Send this tuple away now.
		try {
			output.submit(outputTuple);
		} catch (Exception ex) {
			ex.printStackTrace();
		} // End of try-catch block.	
	} // End of process method.
	
	/*
	Dummy method to demonstrate logging an exception in the process method.
	@throws Exception
	*/
	private void dummyMethodToThrowAnException() throws Exception {
		// Throw a dummy exception that will caught inside the process method above.
		throw new Exception();
	}
	
} // End of ComplexTupleTypeSubmission class.
