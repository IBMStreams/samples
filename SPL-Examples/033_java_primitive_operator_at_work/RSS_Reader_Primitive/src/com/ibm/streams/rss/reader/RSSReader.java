/*
This is a companion Java project for an SPL project (033_java_primitive_operator_at_work)
present in the same Eclipse workspace. This Java class extends from a Streams AbstractOperator
class. It receives an input tuple with rss provider information. Then, it queries that server
for the currently available RSS feeds. It parses the RSS XML data and converts that into 
an output tuple and sends it on its output port.

Since the Java class from this Java project forms the basis for the Java primitive operator 
of the above-mentioned SPL project, the compiled Java class file from this Java project 
should be manually copied to the impl/java/bin directory in the SPL project. 
Under that bin directory, you are required create the necessary sub-directories
that accurately reflect the package name of this java class.
Please read more detailed comments documented in the Main.spl file in the SPL project.

IMPORTANT:
This Java class is extended from a Streams AbstractOperator and in addition it uses
a lot of other Streams Java operators such as the Tuple class. In order to resolve those
Streams Java classes and compile it correctly, you have to do the following.

a) Ensure you are in the Eclipse Java perspective.
b) Right click on the RSS_Reader_Primitive Java project and select Properties.
c) In the left pane of the resulting dialog, click on "Java Build Path".
d) In the righ pane, click on the tab titled "Libraries".
e) Click the "Add External Jars" button, add the following Streams jar files from you
   Streams installation directory.
   
   i)   <Your Streams Install Directory>/lib/com.ibm.streams.operator.jar
   ii)  <Your Streams Install Directory>/lib/com.ibm.streams.operator.samples.jar

f) After adding these three dependent jar files, you can delete/remove the three
erroneous jar entries in your "Libraries" tab that point to invalid directories.
*/
package com.ibm.streams.rss.reader;

import com.ibm.streams.operator.AbstractOperator;
import com.ibm.streams.operator.OperatorContext;
import com.ibm.streams.operator.OutputTuple;
import com.ibm.streams.operator.StreamingData;
import com.ibm.streams.operator.StreamingInput;
import com.ibm.streams.operator.StreamingOutput;
import com.ibm.streams.operator.Tuple;

import java.net.URL;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.CharacterData;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class RSSReader extends AbstractOperator{
	public HashMap rssResults; 
	
	@Override
	public synchronized void initialize(OperatorContext context) throws Exception {
		super.initialize(context);
		// Let us use a set to return the rss entries in the output tuple.
		rssResults = new HashMap();
	} // End of the method initialize.
	
	public void process(StreamingInput<Tuple> inputStream, Tuple tuple) {
		// Input tuple will have the following attributes.
		// 1) String rssProviderName
		// 2) String rssProviderURL
		// 3) int maxRssItemsToBeReturned
		
		// At first, let us clear all the entries in our HashMap.
		rssResults.clear();
		// Get the RSS entries now using the RSS provider URL.
		// Get only the requested number of RSS items.
		getRSSEntries(tuple.getString("rssProviderURL"), tuple.getInt("maxRssItemsToBeReturned"));

		boolean rssEntriesReturned = true;
		
		// If we didn't get any RSS results, set the error indicator.
		if (rssResults.size() <= 0) {
			rssEntriesReturned = false;
			// Send this one tuple away since we have no RSS results from this provider.
			StreamingOutput<OutputTuple> output = getOutput(0);
			OutputTuple outputTuple = output.newTuple();
			outputTuple.setString("rssProviderName", tuple.getString("rssProviderName"));
			outputTuple.setString("rssProviderURL", tuple.getString("rssProviderURL"));
			outputTuple.setInt("maxRssItemsToBeReturned", tuple.getInt("maxRssItemsToBeReturned"));
			outputTuple.setBoolean("rssEntriesReturned", rssEntriesReturned);
			outputTuple.setSet("rssResults", new TreeSet());
	
			try {
				output.submit(outputTuple);
			} catch (Exception ex) {
				ex.printStackTrace();
			} // End of try-catch block.	
		} // End of if (rssEntries.size() <= 0)
		
		// Loop through the hash and send out the RSS results as
		// individual tuples.
		Set set = rssResults.entrySet();
		Iterator iterator = set.iterator();
		
		while (iterator.hasNext()) {
			Map.Entry entry = (Map.Entry) iterator.next();
			TreeSet resultSet = (TreeSet)entry.getValue();

			// We can now prepare an output tuple and send it out.
			StreamingOutput<OutputTuple> output = getOutput(0);
			OutputTuple outputTuple = output.newTuple();
			outputTuple.setString("rssProviderName", tuple.getString("rssProviderName"));
			outputTuple.setString("rssProviderURL", tuple.getString("rssProviderURL"));
			outputTuple.setInt("maxRssItemsToBeReturned", tuple.getInt("maxRssItemsToBeReturned"));
			outputTuple.setBoolean("rssEntriesReturned", rssEntriesReturned);
			outputTuple.setSet("rssResults", resultSet);
	
			try {
				output.submit(outputTuple);
			} catch (Exception ex) {
				ex.printStackTrace();
			} // End of try-catch block.	
		} // End of while (iterator.hasNext())
	} // End of the method process.
	
	public void processPunctuation(StreamingInput<Tuple> inputStream, StreamingData.Punctuation marker) {
		
	} // End of the method processPuntuation

	public void getRSSEntries(String url, int maxRssItemsNeeded) {
		try {
			
			DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
			URL u = new URL(url); // your feed url
				
			Document doc = builder.parse(u.openStream());
			NodeList nodes = doc.getElementsByTagName("item");
			
			for(int i=0;i<nodes.getLength();i++) {
				TreeSet rssResult = new TreeSet();
				
				Element element = (Element)nodes.item(i);
				// Collect the RSS result entry to a List.
				rssResult.add("Title: " + getElementValue(element,"title"));
				rssResult.add("Link: " + getElementValue(element,"link"));
				rssResult.add("Publish Date: " + getElementValue(element,"pubDate"));
				rssResult.add("Author: " + getElementValue(element,"dc:creator"));
				rssResult.add("Comments: " + getElementValue(element,"wfw:comment"));
				rssResult.add("Description: " + getElementValue(element, "description"));
				// Add that set to the results hashmap.
				rssResults.put(i+1, rssResult);
			
				// If we already collected the maximum RSS entries needed, exit this loop now.
				if (i+1 >= maxRssItemsNeeded) {
					break;
				} // End of if (i+1 >= maxRssItemsNeeded)
			
			} // End of for(int i=0;i<nodes.getLength();i++)
		} // End of try
		catch(Exception ex) {
			System.out.println ("RSSReader: Error in accessing the URL: " + url);
		} // End of catch.
	} // End of method getRSSEntries (String url, int maxRssItemsNeeded)

	private String getCharacterDataFromElement(Element e) {
		try {
			Node child = e.getFirstChild();
			if(child instanceof CharacterData) {
				CharacterData cd = (CharacterData) child;
				return cd.getData();
			}
		}
		catch(Exception ex) {
			
		}
		return "";
	} //private String getCharacterDataFromElement

	protected float getFloat(String value) {
		if(value != null && !value.equals("")) {
			return Float.parseFloat(value);
		}
		return 0;
	} // End of getFloat.

	protected String getElementValue(Element parent,String label) {
		return getCharacterDataFromElement((Element)parent.getElementsByTagName(label).item(0));	
	} // End of getElementValue.	
	
} // End of class RSSReader.
