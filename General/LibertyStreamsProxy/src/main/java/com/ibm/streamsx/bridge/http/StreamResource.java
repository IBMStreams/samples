//*******************************************************************************
//* Copyright (C)2018, International Business Machines Corporation and *
//* others. All Rights Reserved. *
//***
package com.ibm.streamsx.bridge.http;
import java.util.List;
import org.apache.http.HttpHost;


public class StreamResource {
	private String ipAddress;   // internal
	private String hostName;  // external
	private int port;	
	private String name;      // service name 
	
	public String getIpAddress() {
		return ipAddress;
	}
		public void setIpAddress(String ipAddress) {
		this.ipAddress = ipAddress;
	}
	public String getHostName() {
		return hostName;
	}
	public void setHostName(String hostName) {
		this.hostName = hostName;
	}
	public void setName(String name) {
		this.name = name;
	}
	
	public int getPort() {
		return port;
	}
	public void setPort(int port) {
		this.port = port;
	}
	public List<String> getTags() {
		return tags;
	}
	public void setTags(List<String> tags) {
		this.tags = tags;
	}
	public String getName() {
		return name;
	}	

	List<String> tags;

	@Override
	public String toString() {
		return(new String(" hostName:" + hostName + " -> ipAddress:" + ipAddress + " port:" + port));		
	}
	public String streamsAddress() {
		return new String(ipAddress+":"+port);
	}
	public HttpHost getDestAddress() {
		HttpHost httpHost = new HttpHost(ipAddress, port, HttpHost.DEFAULT_SCHEME_NAME);
		System.out.println("StreamResource@getDestAddress toHostString() :" + httpHost.toHostString());
		System.out.println("StreamResource@getDestAddress toString() :" + httpHost.toString());		
		return httpHost;
	}
}
