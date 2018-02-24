//*******************************************************************************
//* Copyright (C)2018, International Business Machines Corporation and *
//* others. All Rights Reserved. *
//***
package com.ibm.streamsx.bridge.http;
import java.io.IOException;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.mitre.dsmiley.httpproxy.ProxyServlet;

/**
 * Proxy server extension that derives the proxyHost and proxyPort from 
 * the Streams instance that is bound via VCAP. 
 * 
 * @author siegenth
 *
 */
public class LibertyProxyServlet extends ProxyServlet {
	/**
	 * Usually this will return the value from VCAP
	 */
	private static final long serialVersionUID = 1L;
	private static final String PROXY_DEBUG = "proxyDebug";
	private static final String PROXY_DERIVE = "proxyDerive";	
	private static final String PROXY_HOST = "proxyHost";
	String proxyHostNew = null;
	boolean proxyDebug = false;
	StringBuffer errorMessage = null;
	StreamResource activeStreamResource = null;	
	@Override 
	/**
	 * Get configuration parameters, used by the parent. 
	 * 
	 */
	protected String getConfigParam(String key) {
		System.out.println("key:" + key);
		// return derived ip:port that we derived. 
		if (key.equalsIgnoreCase(P_TARGET_URI)) {
			log("key-override:" + proxyHostNew);
			return proxyHostNew;
		}
		return super.getConfigParam(key);
	  }

	StreamsBlueMix streamsBlueMix = null;
	private StreamResource resolveWithVcap(ServletConfig servletConfig) throws ServletException {
		streamsBlueMix = new StreamsBlueMix();
		StreamResource activeStreamResource = streamsBlueMix.getStreamResourceMapping();
		if (activeStreamResource == null) {
			log("Active StreamResource is null - is Streams up?");
			throw new NoClassDefFoundError();
		}
		log("resolveWithVcap - ipAddress/port/mapping :: " + activeStreamResource.getIpAddress() + "/" + activeStreamResource.getPort()
				+ "/" + activeStreamResource.getHostName());
		return(activeStreamResource);
	}
	
	/*
	 * Initialize the <code>ProxyServlet</code>
	 * Rely on the parent to know what to do, limit the amount of work - just get 
	 * the address from VCAP. Write the mapping into proxyHostNew that can served 
	 * up via getConfigParam()    
	 *
	 * @param servletConfig
	 *            The Servlet configuration passed in by the servlet container
	 * @throws ServletException 
	 */
	@Override
	public void init() throws ServletException {
		boolean getProxyFromVcap = false;

		activeStreamResource = null;
		ServletConfig servletConfig = getServletConfig();
		proxyHostNew = servletConfig.getInitParameter(PROXY_HOST);
		log("init@proxyHostNew is their and proxy [" + proxyHostNew + "] " );		
		String useVcapStr = getConfigParam(PROXY_DERIVE);
		if (useVcapStr != null) {
			getProxyFromVcap = Boolean.parseBoolean(useVcapStr);
		}
		log("init@getProxyFromVcap [" + getProxyFromVcap + "] - flag set we should derive" );		

		if (getProxyFromVcap) {
			activeStreamResource = resolveWithVcap(servletConfig);
			proxyHostNew = activeStreamResource.getDestAddress().toString();
			log(" - new active Host derived from VCAP [" + proxyHostNew + "]" );												
		}
		log(" - init,  activeHost  [" + proxyHostNew + "]" );
		String proxyDebugStr = getConfigParam(PROXY_DEBUG);
		if (proxyDebugStr!= null) {
			proxyDebug= Boolean.parseBoolean(proxyDebugStr);
		}
		log(" - init " + PROXY_DEBUG + "   [" + proxyDebug + "]" );
		
		super.init();
		return;
	}
	@Override
	 protected void service(HttpServletRequest servletRequest, HttpServletResponse servletResponse)
		      throws ServletException, IOException {
		if (errorMessage != null) {
    			log("Error occured : return message");
	    		servletResponse.getWriter().append(errorMessage.toString());
	    		return;
		}

		if (proxyHostNew == null) {
			StringBuffer sb = new StringBuffer("<html><body>");
			sb.append("<h1>Failed to derive host/port.</h1>");
			sb.append("Use the ");
			String url = "http://" + servletRequest.getServerName() + "/TopoDisplay";
			sb.append("<a href=" + '"' + url + '"' + ">"+ url + "</a>");
			sb.append(" servlet for details of derivation of the proxy processing.");			
			sb.append("</body></html>");
    			servletResponse.getWriter().append(sb.toString());
    			return;
			
		}
		if (proxyDebug) {
		    log("contextPath: " +servletRequest.getPathInfo() + " method: " + servletRequest.getMethod() + "test:" + servletRequest.getPathInfo().endsWith(PROXY_DEBUG) + "Proxy:"+ PROXY_DEBUG); 		
		    if (servletRequest.getPathInfo().endsWith(PROXY_DEBUG)) {
		    		// using the already built version - do NOT rederive, use /TopoDisplay for refreshed version
    				StringBuffer sb = new StringBuffer("<html><head></head><body>");
    				sb.append("<h2>Derivation of the current Proxy mapping</h2>");
	    			sb.append(streamsBlueMix.getHtmlProcessing());
	    			String testUrl = streamsBlueMix.buildProxyTest(servletRequest.getScheme(), servletRequest.getServerName(), servletRequest.getServerPort());
	    			if (testUrl != null) {
	    				sb.append(String.format("<br>Proxy configuration success, mapped address to test Streams test point <a href=\"%s\">%s</a>.", testUrl, testUrl));
	    			} else {
	    				sb.append(String.format("<br>Proxy configuration failed - unable to map to the Streams instance running in Cloud"));
	    			}
		    		sb.append("</body></html>");
	    			servletResponse.getWriter().append(sb.toString());	    			
	    			return;
		    }
		}
		super.service(servletRequest, servletResponse);
	}
	
	
}
