package com.acme.test.rest;

import com.ibm.streams.function.model.Function;
import java.io.*;
import java.net.*;
import javax.net.ssl.*;
import javax.xml.bind.DatatypeConverter;

public class MyJavaRestFunctions {
	// ===========================================================================================
	// You must change the following values to suit your Streams environment.
	// Otherwise, the code in the following Java native functions will not work.
	//
	// Specify your Linux user name that owns the Streams instance.
	static final String userName = "streamsadmin";
	// Specify your Linux password.
	static final String password = "passw0rd";
	// Specify your home directory.
	static final String homeDirectory = "/home/streamsadmin";
	// Specify your Streams domain name.
	static final String domainName = "StreamsDomain";
	// Specify your Streams instance name.
	static final String instanceName = "StreamsInstance";
	// Specify the hostname for your SWS (This value can be obtained by running this command: streamtool geturl -i <YOUR_STREAMS_INSTANCE>
	static final String fullyQualifiedHostName = "streamsqse.localdomain";
	// Specify the SWS port number.
	static final String swsPortNumber = "8443";
	// Specify the trust store file name.
	// In Streams 4.x, this store must be obtained manually using the following streamtool command and
	// stored in a directory accessible by this Java code.
	// streamtool exportkeystore -d <YOUR_STREAMS_DOMAIN_NAME> -p <TRUST_STORE_PASSWORD_YOU_WANT_TO_USE>
	//
	static final String trustStore = homeDirectory + "/streams_keystore.jks";
	// Specify the key store password. (Following is the password you gave in the streamtool command you ran above.)
	static final String trustStorePassword = "passw0rd";
	// ===========================================================================================
		
	@Function(name="getStreamsRootResourceInfo", description="Get the Streams root resource information", namespace="com.acme.myrestfunctions")
	public static String getStreamsRootResourceInfo() {
		String urlString = "https://" + fullyQualifiedHostName + ":" + swsPortNumber + "/streams/rest/resources";
		return(makeRestCall(urlString));
	}
	
	@Function(name="getStreamsInstanceInfo", description="Get the Streams instance information", namespace="com.acme.myrestfunctions")
	public static String getStreamsInstanceInfo() {
		String urlString = "https://" + fullyQualifiedHostName + ":" + swsPortNumber + "/streams/rest/instances";
		return(makeRestCall(urlString));
	}
	
	@Function(name="getStreamsHostInfo", description="Get the Streams host information", namespace="com.acme.myrestfunctions")
	public static String getStreamsHostInfo() {
		// String urlString = "https://" + fullyQualifiedHostName + ":" + swsPortNumber + "/streams/rest/instances/" + instanceName + "%40" + userName + "/hosts";
		String urlString = "https://" + fullyQualifiedHostName + ":" + swsPortNumber + "/streams/rest/domains/" + domainName + "/hosts";
		return(makeRestCall(urlString));
	}	
	
	public static String makeRestCall(String urlString) {
	    try {
	        // Identify locations of server truststore and client keystore
	        System.setProperty("javax.net.ssl.trustStore", trustStore);
	        System.setProperty("javax.net.ssl.trustStorePassword", trustStorePassword);
	    	
	        HostnameVerifier hv = new HostnameVerifier() {
	            public boolean verify(String urlHostName, SSLSession session) {
	                return true;   // trust all hosts that supply the Streams certificate
	            }
	        };	        
	        
	        HttpsURLConnection.setDefaultHostnameVerifier(hv);
	        
	        // Retain cookies between requests
	        CookieManager cm = new CookieManager();
	        CookieHandler.setDefault(cm);

	        // Retrieve the root resource information for Infosphere Streams
	        URL url = new URL(urlString);
	        String userInfo  = userName + ":" + password;
	        String authToken = "Basic " + DatatypeConverter.printBase64Binary(userInfo.getBytes());
	        HttpsURLConnection conn = (HttpsURLConnection) url.openConnection();
	        
	        conn.setRequestProperty("Authorization", authToken);
	        conn.setRequestMethod("GET"); 
	        conn.connect();
	        // System.out.println("Response code: " + conn.getResponseCode());
	        // System.out.println("Content type: " + conn.getHeaderField("Content-Type"));
	        String response = new BufferedReader(new InputStreamReader(conn.getInputStream())).readLine();
	        conn.disconnect();
	        return(response);
	      }
	      catch (Exception e) {
	        return(e.getMessage()); 
	      }
	}
}
