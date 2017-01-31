/*
This is a stand-alone Java project that encapsulates the RSS reader function.
Before the RSS reader logic can be added to the SPL Java primitive operator, this 
stand-alone Java project is used to verify and test the business logic. Once it is
functionally working, the core logic is ready to be moved into the process function
of the SPL Java primitive operator.
*/

import java.net.URL;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.CharacterData;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;


public class RSSReader {
	private static RSSReader instance = null;

	private RSSReader() {
	}

	public static RSSReader getInstance() {
		if(instance == null) {
			instance = new RSSReader();
		}
		return instance;
	} // End of method getInstance	
	
	public void writeNews() {
		try {
			
			DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
			URL u = new URL("http://www.economist.com/rss/business_rss.xml"); // your feed url
				
			Document doc = builder.parse(u.openStream());
			NodeList nodes = doc.getElementsByTagName("item");
			
			for(int i=0;i<nodes.getLength();i++) {
				
				Element element = (Element)nodes.item(i);
				System.out.println("Title: " + getElementValue(element,"title"));
				System.out.println("Link: " + getElementValue(element,"link"));
				System.out.println("Publish Date: " + getElementValue(element,"pubDate"));
				System.out.println("Author: " + getElementValue(element,"dc:creator"));
				System.out.println("Comments: " + getElementValue(element,"wfw:comment"));
				System.out.println("Description: " + getElementValue(element, "description"));
				System.out.println();
			}//for
		}//try
		catch(Exception ex) {
			ex.printStackTrace();
		} // End of catch.
	} // End of writeNews

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
	
	
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		RSSReader reader = RSSReader.getInstance();
		reader.writeNews();
	} // End of main.
} // End of the class RSSReader.
