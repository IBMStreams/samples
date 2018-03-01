//*******************************************************************************
//* Copyright (C)2018, International Business Machines Corporation and *
//* others. All Rights Reserved. *
//***
package com.ibm.streamsx.bridge.http;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.stream.Stream;

import com.ibm.json.java.JSONArray;
import com.ibm.json.java.JSONObject;


public class StreamResources {
	StreamResource streamResource = null;		
	List<StreamResource> resourceList = null;
	List<StreamResource> resourceReverseList = null;

	JSONObject streamsResource = null;

	JSONArray specs;
	StreamResources(String resourceString) {
		System.out.println("StreamResources@resourceString: " + resourceString);			
		try {
			streamsResource = JSONObject.parse(resourceString);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		resourceList = new ArrayList<StreamResource>();
		JSONArray resources = (JSONArray)streamsResource.get("resources");
		if (resources == null) {
			System.err.println("Resources value is null, Streams instance not found");
			return;
		}
		System.out.println("StreamResources@resources: " + resources);	
		System.out.println("StreamResources@size: " + resources.size());
		for(Iterator<JSONObject> rIter = resources.iterator(); rIter.hasNext();) {
			streamResource = new StreamResource();
			JSONObject rObj = rIter.next();
			streamResource.setIpAddress(rObj.get("ip-address").toString());
			streamResource.setHostName(rObj.get("host-name").toString());

			streamResource.tags = new ArrayList<String>();			
			JSONArray tags = (JSONArray) rObj.get("tags");
			for (int idx = 0; idx != tags.size(); idx++) {
				streamResource.tags.add(tags.get(idx).toString());
			}
			resourceList.add(streamResource);
		}
	}

	public boolean valid() {
		return resourceList != null;
	}
	public Iterator<StreamResource>getStreamResources() {
		Stream<StreamResource> streams = resourceList.stream();
		return(streams.iterator());
	}
	
	String toHtml() {
		StringBuffer sb = new StringBuffer();
		if (streamResource == null) {
			sb.append("<b>Failed to fetch Streams Resource<b>, possible issues:<br>");
			sb.append("Streams has not been connected with JavaLiberty CloudFoundry App<br>");			
			sb.append("Streams resource is not running.<br>");						
			sb.append("Streams resource has been disabled  <br>");						
		} else {
			for(Iterator<StreamResource> iter = getStreamResources(); iter.hasNext();) {		
				StreamResource sr = iter.next();
				sb.append(sr.getIpAddress() + "->");
				sb.append(sr.getHostName() + "<br>");			
				sb.append(sr.tags + "<br>");						
			}
		}
		sb.append("<br>");
		return sb.toString();				
	}
	List<StreamResource> getResources() {
		return resourceList;
	}
}
