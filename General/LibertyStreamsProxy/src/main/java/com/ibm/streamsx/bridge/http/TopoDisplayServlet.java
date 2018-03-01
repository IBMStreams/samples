//*******************************************************************************
//* Copyright (C)2018, International Business Machines Corporation and *
//* others. All Rights Reserved. *
//***
package com.ibm.streamsx.bridge.http;

import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.ibm.streamsx.rest.Job;
import com.ibm.streamsx.rest.Operator;
import com.ibm.streamsx.rest.Metric;

import com.ibm.streamsx.rest.StreamingAnalyticsConnection;
import com.ibm.streamsx.topology.context.AnalyticsServiceProperties;


/**
 * Servlet for debugging debugging.  Use this to work out how to get the
 * resource ip of the port that the INET toolkit's operators are using. 
 * 
 * This servlet is here only to help out with the the translation. Delete when it is no longer useful. 
 */
@WebServlet(description = "Locate Streams application's streams port number and IP ", urlPatterns = { "/TopoDisplay" })
public class TopoDisplayServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public TopoDisplayServlet() {
        super();
    }
	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		StreamsBlueMix sbm = new StreamsBlueMix();
		sbm.getStreamResourceMapping();
		StringBuffer sb = new StringBuffer("<html><head><title>TopoDisplayServlet</title></head><body>");
		sb.append("<h2>Dervive Proxy and display - this will NOT overwrite active mapping.</h2>");
		sb.append(sbm.getHtmlProcessing());
		String testUrl = sbm.buildProxyTest(request.getScheme(), request.getServerName(), request.getServerPort());
		if (testUrl != null) {
			sb.append(String.format("<br>Proxy configuration success, mapped address to test Streams test point <a href=\"%s\">%s</a>.", testUrl, testUrl));
		} else {
			sb.append(String.format("Proxy configuration failed"));
		}
		sb.append("</body></html>");
		response.getWriter().append(sb.toString());
	}

	/*
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}
