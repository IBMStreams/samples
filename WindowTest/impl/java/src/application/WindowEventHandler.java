package application;

import java.util.Iterator;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;


import com.ibm.streams.operator.Tuple;
import com.ibm.streams.operator.window.StreamWindowEvent;
import com.ibm.streams.operator.window.StreamWindowListener;



public class WindowEventHandler implements StreamWindowListener<Tuple>{

	private static Logger TRACE = Logger.getLogger(WindowEventHandler.class);
	private static String header = "WindowEventTracer ";
	@Override
	public void handleEvent(StreamWindowEvent<Tuple> event) throws Exception {
		
		String tuples = "";
		for (Iterator<Tuple> i = event.getTuples().iterator();i.hasNext();) {
			tuples += i.next().toString();
		}
		
		switch (event.getType()) {
		case INSERTION:
			TRACE.log(Level.TRACE, header +"Insertion event:"+tuples+"\n");
			break;
		case INITIAL_FULL:
			TRACE.log(Level.TRACE,header + "Initial full event:"+tuples+"\n");
			break;
		case TRIGGER:
			TRACE.log(Level.TRACE, header +"Trigger event:"+tuples+"\n");
			break;
		case PARTITION_EVICTION:
			TRACE.log(Level.TRACE, header +"Partition eviction event:"+tuples+"\n");
			break;
		case EVICTION:
			TRACE.log(Level.TRACE, header +"Eviction event:"+tuples+"\n");
			break;
		case FINAL:
			TRACE.log(Level.TRACE, header +"Final event:"+tuples+"\n");
			break;
		}
	}
	

}
