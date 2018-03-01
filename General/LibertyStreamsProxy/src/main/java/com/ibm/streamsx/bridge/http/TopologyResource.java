//*******************************************************************************
//* Copyright (C)2018, International Business Machines Corporation and *
//* others. All Rights Reserved. *
//***
package com.ibm.streamsx.bridge.http;

import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import com.ibm.streamsx.rest.Job;
import com.ibm.streamsx.rest.Metric;
import com.ibm.streamsx.rest.Operator;
import com.ibm.streamsx.rest.StreamingAnalyticsConnection;
/**
 * Derive the IP and PORT using the topology interface when running within BlueMix. 
 * 
 * @author siegenth
 *
 */

public class TopologyResource {
	private static final String METRIC_PORT = "serverPort";
	private static final String KIND_PREAMBLE = "com.ibm.streamsx.inet.res";
	long port;
	String ip = null;
	boolean state = false;
	StringBuffer recordPortFetch =  null;
	
	/**
	* Derive the IP and PORT
	*/
	public TopologyResource() {
		recordPortFetch = new StringBuffer("<h2>TopoloyResource() deriving port....</h2>");
		recordPortFetch.append("<table border=\"1\">");
		state = this.fetchPort();
		recordPortFetch.append("</table>");		
	}
	
	/**
	 * Retrieve the port of the Jetty service within the inet operator. 
	 * @return
	 */
	public boolean getState() {
		return state;
	}
	/**
	 * Keep track of processing steps in an HTML formatted buffer. 
	 * Log the derivation process. 
	 *  
	 * @return string
	 */
	public String recordedPortFetch() {
		return(recordPortFetch.toString());
	}
	/**
	 * Is the port good? 
	 * @return
	 */
	public boolean isPort() {
		return (port != 0);
	}
	public long getPort() {
		if (port == 0) {
			throw new IllegalArgumentException(
				"Failed to locate port that the INET toolkit is using, does the application utilzie the INET toolit?");
		}
		return port;
	}
	/** 
	 * State of the mapping request. 
	 * @return false is bad
	 */

	/*
	 * Currently this portion of the interface is not working, waiting for the 
	 * components to materialize, access to the IP via the resource is still 
	 * needed.
	 * 
	 *   TODO * retieve the IP address that JETTY is running on. 
	 */
	public boolean isIp() {
		return (ip!=null);
	}
	public String getIp() {
		if (ip == null) {
			throw new IllegalArgumentException(
					"Not yet implement, need REST/Resources functionality filled out.");
		}
		return (ip);
	}

	private boolean fetchPort() {
		recordPortFetch.append(String.format("<tr><td>Fetching variable</td><td>%s</td></tr>", StreamsBlueMix.SERVICE_NAME ));
		String vcapString = System.getenv(StreamsBlueMix.SERVICE_NAME);
		if (vcapString == null) {
			recordPortFetch.append(String.format("<tr><td>FAILED to fetch variable </td><td>%s</td></tr>", StreamsBlueMix.SERVICE_NAME ));
			return(false);
		}
		recordPortFetch.append(String.format("<tr><td>%s</td><td>%s</td></tr>", StreamsBlueMix.SERVICE_NAME, vcapString ));		
		StreamingAnalyticsConnection streamingAnalyticsConnection = null;
		StreamsBlueMix streamsBlueMix = null;
		try {
			streamsBlueMix = new StreamsBlueMix();
		} catch (IllegalArgumentException e) {
			recordPortFetch.append(String.format("<tr><td>IllegalArgumentException</td><td>%s</td></tr>", e.getMessage() ));
			return(false);
		}
		if (null == streamsBlueMix.getAnalyticsSeviceName()) {
			recordPortFetch.append(String.format("<tr><td>FAILED to fetch the service name</td><td> Have you bound the Streams to the Foundry App?</td></tr>"));
			return(false);
		}
		recordPortFetch.append(String.format("<tr><td>StreamsBlueMix fetched service name</td><td>%s</td></tr>", streamsBlueMix.getAnalyticsSeviceName()));					
		try {
			// hack for now.
			try {
				streamingAnalyticsConnection = StreamingAnalyticsConnection.createInstance(vcapString, streamsBlueMix.getAnalyticsSeviceName());
			} catch (IllegalStateException e) {
				recordPortFetch.append(String.format("<tr><td>IllegalStateException</td><td>%s</td></tr>", e.getMessage() ));
				return(false);
			}
			if (streamingAnalyticsConnection == null) {
				recordPortFetch.append(String.format("<tr><td>?Problem?</td><td>streamingAnaliticsConnection is null</td></tr>" ));
				return(false);
			}

			List<Job> allJobs = streamingAnalyticsConnection.getInstance().getJobs();
			recordPortFetch.append(String.format("<tr><td>Number of jobs to scan</td><td>%d</td></tr>", allJobs.size()));						
			for (Iterator<Job> jobs = allJobs.iterator(); jobs.hasNext();) {
				Job job = (Job) jobs.next();
				recordPortFetch.append(String.format("<tr><td>*JobName</td><td>%s</td></tr>", job.getName()));										
				for (Iterator<Operator> ops = job.getOperators().iterator(); ops.hasNext();) {
					Operator opr = ops.next();
					recordPortFetch.append(String.format("<tr><td>++Operator</td><td>%s</td></tr>", opr.getName()));															
					for (Metric met : opr.getMetrics()) {
						recordPortFetch.append(String.format("<tr><td>---Metrics</td><td>%s</td></tr>", met.getName()));															
						if (met.getName().equalsIgnoreCase(METRIC_PORT)) {
							recordPortFetch.append(String.format("<tr><td>!!!Metric '%s' located</td><td>operatorKind:%s<br>value:%d</td></tr>", met.getName(), opr.getOperatorKind(), met.getValue()));
							if (opr.getOperatorKind().startsWith(KIND_PREAMBLE)) {
								port = met.getValue();
								recordPortFetch.append(String.format("<tr><td>Success</td><td>port located %d</td></tr>", port));								
								state = true;
							}
						}
					}
				}
			}
		} catch (IOException e) {
			System.err.println("Failing to retieve data via the REST interface.");
			e.printStackTrace();
			recordPortFetch.append("<tr><td>ERROR</td><td>Failed to retieve data via the REST</td></tr>");
			return false;
		}
		recordPortFetch.append(String.format("<tr><td>Port derivation completion state</td><td>%b</td></tr>", state));										
		return state;
	}
}
