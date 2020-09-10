//*******************************************************************************
//* Copyright (C)2018, International Business Machines Corporation and *
//* others. All Rights Reserved. *
//***
package com.ibm.streamsx.bridge.http;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import javax.servlet.ServletException;

import com.ibm.json.java.JSONArray;
import com.ibm.json.java.JSONObject;

/**
 * StreamsBlueResource - Support for Streams when running within IBM Cloud.
 * Streams infrastructure was conceived as running on the bare hardware. When
 * it's running within IBM Cloud, this is not the case, some 'glue' is needed to
 * bridge the gap between expectations and reality.
 * 
 * 
 * <ul>
 * <li>proxy HTTP, from the wide web into Streams IBM Cloud.</li>
 * <li>proxy WS, from the wide web into Streams running within IBM Cloud</li>
 * <li>resources within in IBM Cloud,</li>
 * </ul>
 * 
 * @author siegenth
 *
 */
public class StreamsBlueMix  {
	public static final String SERVICE_NAME = "VCAP_SERVICES";
	private static final String STREAMS_NAME = "streaming-analytics";
	private static final String ANALYTICS_NAME = "name";
	private static final String INET_RESOURCES_DEFAULT = "/streamsx.inet.resources";  // default provided by toolkit. 
	StreamResource activeStreamResource;

	List<String> logList = null;

	/**
	 * Accept the vcapString that was gotten from the enviroment and parse it.
	 * It should have the connection information (url/path/user/pass) use to the
	 * Streams resource.
	 * 
	 * @param vcapString
	 * @return
	 */
	JSONObject serviceCloud = null;
	JSONObject serviceStreams = null;
	private String analyticsServiceName = null;

	public StreamsBlueMix(String service) {
		logList = new ArrayList<String>();
		String vcapString = System.getenv(service);
		if (vcapString == null) {
			throw new IllegalArgumentException(
					"Failed to get environment variable '" + service + "', a value that is specified by IBM Cloud.");
		}
		JSONObject vcap = null;
		try {
			vcap = JSONObject.parse(vcapString);
		} catch (IOException e) {
			throw new IllegalArgumentException("Failed to parse value from '" + service + "' : " + vcapString);
		}
		System.out.println("start VCAP processing...." + vcap);
		for (Object key : vcap.keySet()) {
			String keyStr = (String) key;
			if (keyStr.contains(STREAMS_NAME)) {
				serviceStreams = (JSONObject) ((JSONArray) vcap.get(keyStr)).get(0);
				System.out.println("streaming-analytics :: " + serviceStreams.toString());
				if (!serviceStreams.containsKey(ANALYTICS_NAME)) { 
					throw new IllegalArgumentException("Failed to retrieve '" + ANALYTICS_NAME + "'from '" + STREAMS_NAME + "' entry of services:" + service);					
				}
				analyticsServiceName = serviceStreams.get(ANALYTICS_NAME).toString();
				break;
			}
		}
	}

	public StreamsBlueMix() {
		this(SERVICE_NAME);
	}

	public String getAnalyticsSeviceName() {
		return analyticsServiceName;
	}
	
	private StreamCredential getStreamCredential() {
		StreamCredential streamCredential = null;

		if (serviceStreams != null) {
			JSONObject creds = (JSONObject) serviceStreams.get("credentials");
			logList.add("CREDENITALS");
			streamCredential = new StreamCredential(creds);
		} else {
			System.err.println("Failed to locate 'streaming-analytics in VCAP environment variable.");
			streamCredential = null;
		}
		return (streamCredential);
	}
	String valididatedUrlString = null;
	private static int ACTIVE_TEST_TIMEOUT = 10000;

	/**
	 * See if the ip/port is active, if it is then were assuming that it is the
	 * port that Streams is running on. This did not work as I expected, no
	 * matter what it returned true. 
	 * 
	 * @param ip
	 * @param port
	 * @return
	 */
	boolean isInetActive(String ip, int port) {
		boolean online = true;
		if (true) {
			SocketAddress sockaddr = new InetSocketAddress(ip, port);
			Socket socket = new Socket();
			try {
				socket.connect(sockaddr, ACTIVE_TEST_TIMEOUT);
				online = true;
				socket.close();
			} catch (IOException IOException) {
				online = false;
			}
		}
		System.out.println(ip + ":" + port + " connection is active: " + online);
		return (online);
	}

	/**
	 * Check to see if the connection can be made. The isInetActive() makes more
	 * 
	 * @param ip
	 * @param port
	 * @return
	 */
	private boolean checkHttpValid(String ip, int port, String path) {
		URL url = null;
		HttpURLConnection huc = null;
		String urlString = "http://" + ip + ":" + Integer.toString(port) + path;
		System.out.println("isStatusValid@checking" + urlString);
		try {
			url = new URL(urlString);
			huc = (HttpURLConnection) url.openConnection();
			huc.setRequestMethod("GET"); // OR
			huc.connect();
			int code = huc.getResponseCode();
			System.out.println("isHttpValid@valid " + urlString + " code:" + code);
			huc.disconnect();
			valididatedUrlString = urlString;
			return (true);
		} catch (MalformedURLException e) {
			System.err.println(urlString + ": isStatusValid@failed MalformedURLException");
		} catch (ProtocolException e) {
			System.err.println(urlString + ": isStatusValid@failed ProtocolException");
		} catch (IOException e) {
			System.err.println(urlString + ": isStatusValid@failed IOException ");
		}
		if (huc != null) {
			huc.disconnect();
		}
		return (false);
	}
	
	
	/**
	 * Check to see if we have connectivity, attempt multiple avenues.  
	 * 
	 * @param ip
	 * @param port
	 * @return boolean 
	 */
	private boolean isHttpValid(String ip, int port) {
		if (checkHttpValid(ip, port,INET_RESOURCES_DEFAULT)) { 
			return true;
		}
		if (checkHttpValid(ip, port,"/")) {
			return true;
		}		
		return false;
	}
	
/**
 * Find the Streams resource:
 * Get resources that  Streams has got, by default their usually two nodes.
 * Then hunt through the nodes port to get a connection, when you get an
 * connection were assuming that his is node and port that all the http requests
 * will be going through.  
 * 
 * 	
 * @return valid StreamResource
 */
	TopologyResource topologyResource = null;
	StringBuffer recordIpSearch = null; 
	public StreamResource getStreamResourceMapping() throws ServletException {
		StreamCredential streamCredential = getStreamCredential();
		String resourceString = streamCredential.getStreamsResource();
		StreamResources streamResource = new StreamResources(resourceString);
		logList.add(streamResource.toHtml());
		StreamResource activeSR = null;
	    topologyResource = new TopologyResource();
		if (topologyResource.isPort() ) {
			int portCheck = (int)topologyResource.getPort();			
			recordIpSearch = new StringBuffer(String.format("<h2>Hunting for IP address using port:%d </h2>", portCheck));
			recordIpSearch.append(getLogging());
			if (!topologyResource.isIp()) {
				for (Iterator<StreamResource> iter = streamResource.getStreamResources(); iter.hasNext();) {
					activeSR = iter.next();
					recordIpSearch.append(String.format("- Check active Streams resource, checking:%s agaist port %d to %s <br>", activeSR.getIpAddress(), portCheck, activeSR.getHostName()));
					if (isHttpValid(activeSR.getIpAddress(), portCheck)) {
						activeSR.setPort(portCheck);
						activeStreamResource = activeSR;
						recordIpSearch.append(String.format("<h3>Dervived Streams resource mapping : %s </h3>", activeStreamResource.toString()));
						recordIpSearch.append(String.format("URL used to test <b>internal</b> site %s.", valididatedUrlString));
						break;
					}
					recordIpSearch.append(String.format(" ** Failed Streams Resource Mapping<br>"));
				}
			} else {
				throw new RuntimeException("TopologyResource has returning an IP, if this is correct then you should use the value here.");
			}
		}
		return activeStreamResource;
	}

	public String getLogging() {
		StringBuffer stringBuffer = new StringBuffer();
		logList.add("<hr>");
		logList.add("The mapping is..");
		logList.add(activeStreamResource != null ? activeStreamResource.toString() : "NO activeStreamResource");
				logList.forEach((temp) -> {
					stringBuffer.append(temp);
					stringBuffer.append("<br>");
				});
		return (stringBuffer.toString());
	}
	/**
	 * Return HTML page logging the steps todo the mapping. 
	 * This is for debugging work out what is not working.  
	 */
	public String getHtmlProcessing() {
		StringBuffer sb = new StringBuffer();
		if (topologyResource != null) {
			sb.append(topologyResource.recordedPortFetch());
		} else {
			sb.append(String.format(" ** FAIL-IP ** IP could not be derivied from VCAP<br>"));						
		}
		if (recordIpSearch != null) {
			sb.append(recordIpSearch.toString());
		} else {
			sb.append(String.format(" ** FAIL-HUNT ** No IP Hunt performed, no port could be retrieved<br>"));			
		}
		return sb.toString();
	}

	public String buildProxyTest(String scheme, String serverName, int serverPort) {
		if (valididatedUrlString != null) {
			return(String.format("%s://%s:%d%s",scheme, serverName, serverPort, INET_RESOURCES_DEFAULT));
		}
		return null;
	}
}
