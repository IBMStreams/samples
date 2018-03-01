//*******************************************************************************
//* Copyright (C)2018, International Business Machines Corporation and *
//* others. All Rights Reserved. *
//***

package com.ibm.streamsx.bridge.http;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.Base64;

import com.ibm.json.java.JSONObject;

class StreamCredential {
	String userid;
	String password;
	String rest_url;
	String resource_path;
	
	StreamCredential(JSONObject credJson) {
		userid = credJson.get("userid").toString();
		password = credJson.get("password").toString();				
		rest_url = credJson.get("rest_url").toString();								
		resource_path = credJson.get("resources_path").toString();																
	}
	StreamCredential( String rest_url, String resource_path, String userid, String password) {
		this.userid = userid;
		this.password = password;
		this.rest_url = rest_url;
		this.resource_path = resource_path;
	}
	
	
	String getStreamsResource() {
		String resourceString = null; 
        try {
            URL url = new URL (this.rest_url + this.resource_path);
            final String secure= this.userid + ":" + this.password;
            final byte[] authBytes = secure.getBytes(StandardCharsets.UTF_8);
            String encoding = Base64.getEncoder().encodeToString(authBytes);

            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            connection.setDoOutput(true);
            connection.setRequestProperty  ("Authorization", "Basic " + encoding);
            InputStream content = (InputStream)connection.getInputStream();
            
            BufferedReader in   = 
                new BufferedReader (new InputStreamReader (content));
            String line;
            
            while ((line = in.readLine()) != null) {
            	if (resourceString==null) {
            		resourceString = line;
            	} else {
            		System.err.println("Multiple resource String(s) : " + line );
            	}
            }
        } catch(Exception e) {
            e.printStackTrace();
        }
        return (resourceString);
	}	
	
	String toHtml() {
		StringBuffer sb = new StringBuffer();
		sb.append("userid").append("::").append(userid).append("<br>");				
		sb.append("password").append("::").append(password).append("<br>");				
		sb.append("rest_url").append("::").append(rest_url).append("<br>");					
		sb.append("resources_path").append("::").append(resource_path).append("<br>");
		return(sb.toString());
	}	
}
