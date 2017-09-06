package com.ibm.streams.cloud.samples;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Logger;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.core.UriInfo;
import java.net.URI;
import javax.xml.bind.DatatypeConverter;

import org.apache.http.HttpEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpDelete;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.entity.mime.MultipartEntityBuilder;
import org.apache.http.entity.mime.content.FileBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;

import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.ssl.SSLContexts;
import javax.net.ssl.SSLContext;
import org.apache.http.conn.ssl.TrustSelfSignedStrategy;
import org.apache.http.conn.ssl.NoopHostnameVerifier;

import com.ibm.json.java.JSONArray;
import com.ibm.json.java.JSONObject;


@Path("/jax-rs/")
public class TrafficResource {
	   private static final Logger LOGGER = Logger.getLogger(TrafficResource.class.getName());
	   
	   private static final String STREAMS_SERVICE_NAME	= "streaming-analytics";
	   private static Properties configProperties = null;
	   // This is the information provided in the vcap for the bound Streams service 
	    private static String instanceURL = null;
	    private static String statusPath = null;
	    private static String statisticsPath = null;
	    private static String startPath = null;
	    private static String stopPath = null;
	    private static String resourcesPath = null;
	    private static String addResourcePath = null;
	    private static String removeResourcePath = null;
	    private static String uploadPath = null;
	    private static String bundlesPath = null;
	    private static String jobsPath = null;
	    
		private static String userId = null;
	    private static String password = null;
	    private static String api_creds = null;
	    private static String apiKey = null;
	    
	    private static final JSONObject credentials = getCredentials();
	    
	    private static boolean firstTime=true; 
	    private static boolean timedOut = false;
		private static boolean checkJobRunning = false;
	    private static String jobID = "";
		private static String jobName = "";
	    private static LinkedList<String> statusLines = new LinkedList<String>();
	    private static HashMap<String, JSONObject> topNs = new HashMap<String, JSONObject>();
	    
		private static SSLContext sslcontext = null;
		private static SSLConnectionSocketFactory sslSocketFactory = null;

    static {
        try {
        	// Force TLSv1.2 protocol, use NoopHostnameVerifier to trust self-signed cert
            sslcontext= SSLContexts.custom().loadTrustMaterial(null,
                    new TrustSelfSignedStrategy()).build();
            sslSocketFactory = new SSLConnectionSocketFactory(sslcontext,
                new String[] { "TLSv1.2"  }, null, new NoopHostnameVerifier());

        }
        catch (final Exception exception) { 
        	exception.printStackTrace(); 
            LOGGER.severe("ResourceUtils static EXCEPTION: " + exception.getMessage());
        }
    }
	   
	    /**
	     * GET RefreshStatus 
	     * Check if instance and job are running and start if necessary
	     * return the instance and job status lines 
	     */
	    @GET
	    @Path("refreshStatus")
	    @Produces(MediaType.APPLICATION_JSON)
	    public JSONObject getRefreshStatus(@Context UriInfo uriInfo)    {
	    	/*
	    	 * get instance status
	    	 * if not started then start
	    	 * if error starting then error and skip job check 
	    	 * if job not exists
	    	 * start job 
	    	 * *note check job will be done in separate thread 
	    	 * if error  then error
	    	 * else (job not exists ) if job not healthy - report status 
	    	 * return status lines
	    	 */
	    	if (firstTime) {
	    		/* 
	    		 *  make sure instance is started and submit SPL job and start background timer to cancel 
	    		 *  job after 2 hours 	
	    		 */
						
	    		firstTimeInit(uriInfo.getBaseUri());
	    		firstTime=false;
	    	}
	    	
	    	// Return the status lines
	    	JSONArray ja = new JSONArray();
			for (int i = 0; i < statusLines.size(); i++) {
				JSONObject s = new JSONObject();
				s.put("statusLine", statusLines.get(i));
				ja.add(s);
		    }
			
			JSONObject jo = new JSONObject();
			jo.put("refreshStatus", ja);
			
	    	return jo;
	    }
	    
	private void firstTimeInit(URI uriBase) {
			// start the instance if necessary and start the spl job
		JSONObject stat = getStatus();
    	LOGGER.info("Instance Info: " + stat.toString());
    	String instanceStatus = (String) ((JSONObject) stat.get("SAS_status")).get("status");
    	boolean instanceOK = false; 
    	if (instanceStatus.equals("stopped")) {
    		//try to start instance 
    		addStatusLine("Attempt to start instance");
    		JSONObject startIResult = startInstance("{}"); //pass empty json object
    		LOGGER.info("Instance Start Status: " + startIResult.toString());
    		stat = getStatus();
	    	LOGGER.info("Instance Info After starting: " + stat.toString());
	    	instanceStatus = (String) ((JSONObject) stat.get("SAS_status")).get("status");
	    	if (!instanceStatus.equals("running")) {
	    		// Instance didn't Start 
	    		LOGGER.info("Start Instance was unsuccessful: " + stat.toString());
	    		addStatusLine("Start Instance was unsuccessful: " + stat.toString());
	    		instanceOK = false;
	    	} else {
	    		//All is well with instance  
	    		instanceOK=true;
	    		LOGGER.info("Start Instance was successful" );
	    		addStatusLine("Start Instance was successful");
	    	}
    	} else {
    		//Instance is not stopped  
    		LOGGER.info("Instance is not stopped");
    		if (!instanceStatus.equals("running")) {
	    		// Instance isn't healthy because it wasn't stopped or running 
	    		LOGGER.info("Instance is not healthy: " + stat.toString());
	    		addStatusLine("Instance is not healthy: " + stat.toString());
	    		instanceOK=false;
	    	} else {
	    		//All is well with instance  
	    		instanceOK=true;
	    	}
    	}
 
    	if (instanceOK) { 
		  LOGGER.info("uriBase: " + uriBase.toString());
		  LOGGER.info("uriHost: " + uriBase.getHost());
		  
		  jobName = "NYCTraffic_" + uriBase.getHost();  
		  String checkJobResult = checkJob();
		  if (checkJobResult.equals("NO_JOB") | checkJobResult.equals("NO_JOBS") ) {
    		//start job
    		addStatusLine("Attempt to start job");
    		JSONObject submitAPIResult=  getsubmit(uriBase.toString()+"jax-rs/addLine");  
    		JSONObject submitResult = (JSONObject) submitAPIResult.get("submit");
    		LOGGER.info("APIStatus: " + (String) submitAPIResult.get("APIstatus"));
    		if (((String) submitAPIResult.get("APIstatus")).equals("complete")) {
    			LOGGER.info("APi result : " + submitResult);
    			LOGGER.info("APi job id : " + (String) submitResult.get("jobId"));
				jobID = (String) submitResult.get("jobId");
    		} 
    		LOGGER.info("Start Job Result: " + submitResult.toString());
    		addStatusLine("Start Job Result: " + submitResult.toString());  
		  }			
		  /*
    			 * Create a background thread that will time out and cancel job 
    			 */
				 
    			Timer timer = new Timer();
    			long delay = 120*60*1000; // delay in milliseconds (120 minutes) 
    			timer.schedule(new TimerTask() {
    				  @Override
    				  public void run() {
    					  LOGGER.info("The time limit expired: About to cancel job id : " + jobID + "; jobName : " + jobName);
    					  addStatusLine("The time limit expired: About to cancel job id : " + jobID + "; jobName : " + jobName); 
    					  JSONObject  cancelResult = cancelJob(jobID);
    					  LOGGER.info("Cancel Job Result: " + cancelResult.toString());
    			    	  addStatusLine("Cancel Job Result: " + cancelResult.toString()); 
    			    	  if (checkJob().equals("NO_JOBS")) {
    			    		  //no jobs running at all so stop instance 
    			    		  LOGGER.info("No Jobs running; About to stop instance");
        					  addStatusLine("No Jobs running; About to stop instance");
        					  JSONObject stopResult = stopInstance();
        					  LOGGER.info("Stop Instance Result: " + stopResult.toString());
        			    	  addStatusLine("Stop Instance Result: " + stopResult.toString()); 			    		  
    			    	  }
    			    	  timedOut=true;
    				  }
    				}, delay);  

				/*
    			 * Create a background thread that will collect job status  
    			 */
				 
    			Timer timer2 = new Timer();
    			 
				long delay2 = 1*60*1000; // delay in milliseconds (1 minutes)
    			timer2.scheduleAtFixedRate(new TimerTask() {
    				  @Override
    				  public void run() {
    					if (!timedOut) {
							if (!checkJobRunning) { //sometimes calls to service can back up so don't start another unless prior one finished
							  checkJobRunning = true; // indicate we are running
							  checkJob();  
							  checkJobRunning=false; // indicate we finshed running	
							}
						}
    				  }
    				}, delay2, delay2); 					
    	}		
	}
	

	private void addStatusLine(String s) {
		final DateFormat df = new SimpleDateFormat("MM/dd/yy HH:mm:ss");
		statusLines.addFirst(df.format(new Date()) + " - " + s);
		if (statusLines.size() > 201) statusLines.removeLast(); // we'll keep only 200 lines
	}
	
	private String checkJob() {
		//check on job
		JSONObject  stat = getJobs(); 
		LOGGER.info("Jobs Info: " + stat.toString());
		JSONArray jobs = (JSONArray) ((JSONObject) stat.get("jobs")).get("jobs");
		Iterator<JSONObject> itr = jobs.iterator();
		String result = "UNKNOWN";  
		if (itr.hasNext()) {
			// check for traffic job.
			while(itr.hasNext()) {
    	         JSONObject job = (JSONObject) itr.next();
				 LOGGER.info("about to check  Job Info: " + job.get("name").toString() + "with: " + jobName );
				 if ((job.get("name").toString()).equals(jobName)) {
    	        	 LOGGER.info("Traffic Job Info: " + job.toString());
					 jobID = job.get("jobId").toString();
    	        	 if (((String)job.get("health")).equals("healthy")) {
    	        		 result = "HEALTHY";
    	        		 addStatusLine("Job is healthy: " + job.toString());
    	        	 } else {
    	        		 result = "NOT_HEALTHY";
    	        		 addStatusLine("Job was not healthy: " + job.toString());
    	        	 }
    	         }  
    	    }
			if (result.equals("UNKNOWN")) {
				// NYCTraffic job was not in list of jobs returned 
				addStatusLine("Job " + jobName + " was not found.");
				result="NO_JOB";
			}
		} else {
			// no jobs running at all
			result="NO_JOBS";
		}
		return result;
		}

	/**
     * GET 
     * TopTenLists retrieve the top ten lists 
     */
    @GET
    @Path("topTens")
    @Produces(MediaType.APPLICATION_JSON)
    public JSONObject getTopTens()    {	
    	JSONObject topTensObj = new JSONObject();
    	JSONArray tensArray = new JSONArray();
    	for (JSONObject value : topNs.values()) {
    	  tensArray.add(value);
    	}
    	topTensObj.put("TopTens", tensArray);
    	return topTensObj;
    }
	
    /**
	 * POST 
	 * rest api to receive a json string and add to lines 
	*/
	@POST
	@Path("addLine")
	@Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.TEXT_PLAIN)
	public String addLine(final JSONObject         jsonObject) {

		LOGGER.info("POST addLine: add a json line: " + jsonObject.toString());
		JSONObject parsedObj = null;
		try {
			parsedObj = JSONObject.parse((String) jsonObject.get("jsonString"));
		} catch (IOException e) {
			e.printStackTrace();
		}
		topNs.put(parsedObj.get("attributeName").toString(), parsedObj);
		return "Line added";
	}
    
    /**
     * Jobs retrieve the job information 
     */
    public JSONObject getJobs()    {
    	final String url = instanceURL + jobsPath ;
    	final String type = "jobs";
    	final JSONObject result = getGeneric(url, type);
        return result;
    }
    
    /**
     * Status retrieve the status information 
     */
    public JSONObject getStatus()    {
    	final String url = instanceURL + statusPath ;
    	final String type = "SAS_status";
    	final JSONObject result = getGeneric(url, type);
        return result;
    }
    
    
    /**
     * 
     * @param url the url to call
     * @param type - the object to retrieve (eg jobs, resources, status, statistics 
     * @return JSONObject for result of api call 
     */
    public JSONObject getGeneric(String url, String type)    {
        LOGGER.info("GET " + type + " request:");
        final JSONObject result = new JSONObject();
        result.put("APICall", type);
        result.put("APIstatus", "starting");
       
        try
        {
    		result.put(type, url);
            CloseableHttpClient httpclient = HttpClients.custom().
    	            setSSLSocketFactory(sslSocketFactory).build();
            HttpGet httpGet = new HttpGet(url);
            httpGet.addHeader("Authorization", getAPIKey());
            CloseableHttpResponse response1 = httpclient.execute(httpGet);
            try {
                LOGGER.info(response1.getStatusLine().toString());
                HttpEntity entity1 = response1.getEntity();
                
                InputStream is = entity1.getContent();
                JSONObject myResult = JSONObject.parse(is);
                LOGGER.info("Raw json returned is: " + myResult.toString());
                result.put(type,myResult);
                result.put("APIstatus", "complete");
                EntityUtils.consume(entity1); // and ensure it is fully consumed
            } finally {
                response1.close();
            }
        }
        catch (final Exception exception)
        {
        	exception.printStackTrace();
			LOGGER.severe("Get " + type + " EXCEPTION: " + exception.getMessage());
	         exception.printStackTrace();
	            result.put("APIstatus", "exception");
	            result.put("exception", exception);
        }
        LOGGER.info("GET " + type + " response body: " + result);
        return result;
    }
     
    
    /**
     * Cancel job 
     */
    public JSONObject cancelJob(String jobId)    {
        LOGGER.info("GET cancelJob request:");
        final JSONObject result = new JSONObject();
        result.put("APICall", "cancelJob");
        result.put("status", "starting");
       
        try
        {
    		    		
    		final String url = instanceURL + jobsPath + "?job_id=" + jobId ;
            result.put("cancelJob", url);
			CloseableHttpClient httpclient = HttpClients.custom().
    	            setSSLSocketFactory(sslSocketFactory).build();
            HttpDelete httpDelete = new HttpDelete(url);
            httpDelete.addHeader("Authorization", getAPIKey());
            CloseableHttpResponse response1 = httpclient.execute(httpDelete);
            try {
                LOGGER.info(response1.getStatusLine().toString());
                HttpEntity entity1 = response1.getEntity();
                
                InputStream is = entity1.getContent();
                JSONObject cancelJobResult = JSONObject.parse(is);
                result.put("cancelJob",cancelJobResult);
                result.put("status", "complete");
                EntityUtils.consume(entity1); // and ensure it is fully consumed
            } finally {
                response1.close();
            }
        }
        catch (final Exception exception)
        {
        	exception.printStackTrace();
			LOGGER.severe("Get cancelJob EXCEPTION: " + exception.getMessage());
	         exception.printStackTrace();
	            result.put("status", "exception");
	            result.put("exception", exception);
        }
        LOGGER.info("GET cancelJob response body: " + result);

        return result;
    }
    
    /**
     * Start Instance 
     */
    
    public JSONObject startInstance(String startJSON)    {
        LOGGER.info("GET startInstance request:");
        final JSONObject result = new JSONObject();
        result.put("APICall", "startInstance");
        result.put("APIstatus", "starting");
       
        try
        {
    		    		
    		final String url = instanceURL + startPath ;
            result.put("startInstance", url);
			CloseableHttpClient httpclient = HttpClients.custom().
    	            setSSLSocketFactory(sslSocketFactory).build();
            HttpPut httpPut = new HttpPut(url);
            httpPut.addHeader("Authorization", getAPIKey());
            httpPut.addHeader("Content-Type", "application/json");
            
            StringEntity input = new StringEntity(startJSON);
            httpPut.setEntity(input);
            
            CloseableHttpResponse response1 = httpclient.execute(httpPut);
            try {
                LOGGER.info(response1.getStatusLine().toString());
                HttpEntity entity1 = response1.getEntity();
                
                InputStream is = entity1.getContent();
                JSONObject startInstanceResult = JSONObject.parse(is);
                result.put("startInstance",startInstanceResult);
                result.put("APIstatus", "complete");
                EntityUtils.consume(entity1); // and ensure it is fully consumed
            } finally {
                response1.close();
            }
        }
        catch (final Exception exception)
        {
        	exception.printStackTrace();
			LOGGER.severe("Get startInstance EXCEPTION: " + exception.getMessage());
	         exception.printStackTrace();
	            result.put("APIstatus", "exception");
	            result.put("exception", exception);
        }
        LOGGER.info("GET startInstance response body: " + result);

        return result;
    }
    
    /**
     * Stop Instance 
     */
   public JSONObject stopInstance()    {
        LOGGER.info("GET stopInstance request:");
        final JSONObject result = new JSONObject();
        result.put("APICall", "stopInstance");
        result.put("APIstatus", "starting");
       
        try
        {
    		    		
    		final String url = instanceURL + stopPath ;
            result.put("stopInstance", url);
			CloseableHttpClient httpclient = HttpClients.custom().
    	            setSSLSocketFactory(sslSocketFactory).build();
            HttpPut httpPut = new HttpPut(url);
            httpPut.addHeader("Authorization", getAPIKey());
            
            CloseableHttpResponse response1 = httpclient.execute(httpPut);
            try {
                LOGGER.info(response1.getStatusLine().toString());
                HttpEntity entity1 = response1.getEntity();
                
                InputStream is = entity1.getContent();
                JSONObject stopInstanceResult = JSONObject.parse(is);
                result.put("stopInstance",stopInstanceResult);
                result.put("APIstatus", "complete");
                EntityUtils.consume(entity1); // and ensure it is fully consumed
            } finally {
                response1.close();
            }
        }
        catch (final Exception exception)
        {
        	exception.printStackTrace();
			LOGGER.severe("Get stopInstance EXCEPTION: " + exception.getMessage());
	         exception.printStackTrace();
	            result.put("APIstatus", "exception");
	            result.put("exception", exception);
        }
        LOGGER.info("GET stopInstance response body: " + result);

        return result;
    }
    
    /**
     * submit submit SPL Traffic job 
     */ 
    public JSONObject getsubmit(String uri)    {
        LOGGER.info("GET submit request:");
        final JSONObject result = new JSONObject();
        result.put("APICall", "submit");
        result.put("APIstatus", "starting");
        		
       	final String fileName = "NYCTraffic.sab";
       	JSONObject submitJSON = new JSONObject();
		JSONObject parms = new JSONObject();
		parms.put("Top10.host", uri);
		submitJSON.put("submissionParameters", parms);
		JSONObject config = new JSONObject();
		submitJSON.put("configurationSettings", config);
		submitJSON.put("jobName", jobName);
		LOGGER.info("Here's the submit json: " + submitJSON);
		return doSubmit(fileName, submitJSON);
    }
    
    
    /**
     * Helper method to perform submit given a file name and submit json  
     * 
     */
    private JSONObject doSubmit(String fileName, JSONObject submitJSON)    {
        LOGGER.info("doSubmit for file: " + fileName);
        LOGGER.info("doSubmit for json: " + submitJSON.toString());
        
        final JSONObject result = new JSONObject();
        result.put("APICall", "submit");
        result.put("APIstatus", "starting");
       
        try
        {
        	final String fullFileName = "./apps/myapp.war/public/" + fileName;
        	File file = new File(fullFileName);
			LOGGER.info("File to upload is: " + file.getAbsolutePath().toString());
			if(!file.exists()) {
                LOGGER.severe("Response EXCEPTION: Application Not Found: file: " + file.getAbsolutePath() + " ... " + file.getAbsolutePath());
        		throw new WebApplicationException(createJSONResponse(
        				Status.NOT_FOUND, "Application not found"));
            }
    		
    		final String url = instanceURL + jobsPath + "?bundle_id=" + fileName;
            result.put("submit", url);
			CloseableHttpClient httpclient = HttpClients.custom().
    	            setSSLSocketFactory(sslSocketFactory).build();
            HttpPost httpPost = new HttpPost(url);
            httpPost.addHeader("accept", "application/json");
            httpPost.addHeader("Authorization", getAPIKey());
            
            FileBody bin = new FileBody(file);
            LOGGER.info("Here's the submit json: " + submitJSON);
           
            
            StringBody submitJson = new StringBody(submitJSON.toString(), ContentType.APPLICATION_JSON );

            HttpEntity reqEntity = MultipartEntityBuilder.create()
                    .addPart("bin", bin)
                    .addPart("json", submitJson)
                    .build();

            httpPost.setEntity(reqEntity);   
            
            CloseableHttpResponse response1 = httpclient.execute(httpPost);
            try {
                LOGGER.info(response1.getStatusLine().toString());
                HttpEntity entity1 = response1.getEntity();
                
                InputStream is = entity1.getContent();
                JSONObject submitResult = JSONObject.parse(is);
                result.put("submit",submitResult);
                if (response1.getStatusLine().getStatusCode() != 200) {
                	result.put("APIstatus", "failed");
                } else {
                	result.put("APIstatus", "complete");
                }
                EntityUtils.consume(entity1); // and ensure it is fully consumed
            } finally {
                response1.close();
            }
        }
        catch (final Exception exception)
        {
        	exception.printStackTrace();
			LOGGER.severe("Get submit EXCEPTION: " + exception.getMessage());
	         exception.printStackTrace();
	            result.put("APIstatus", "exception");
	            result.put("exception", exception.toString());
        }
        LOGGER.info("GET submit response body: " + result);

        return result;
    }
    
    /**
     * Get  the VCAP credential info from the bound service
     */
    synchronized private static JSONObject getCredentials() {
    	JSONObject result = null;
    	LOGGER.info("System Environment:" + System.getenv());
        try
        {
			Map<String, String> env = System.getenv();
			String vcapServices = env.get("VCAP_SERVICES");
			if(vcapServices != null) {
				JSONObject jsonObject = JSONObject.parse(vcapServices);
		        LOGGER.info("Test VCAP: " + jsonObject.toString());
				JSONArray services = (JSONArray)jsonObject.get(STREAMS_SERVICE_NAME);
				if(services != null) {
					JSONObject service = (JSONObject)services.get(0);
					if(service != null) {
						result = (JSONObject)service.get("credentials");
						if(result != null) {
							userId = (String)result.get("userid");
							password = (String)result.get("password");
							api_creds = userId + ":" + password;
							apiKey = getAPIKey();
							instanceURL = (String)result.get("rest_url");
						    statusPath = (String)result.get("status_path");
						    statisticsPath = (String)result.get("statistics_path");
						    startPath = (String)result.get("start_path");
						    stopPath = (String)result.get("stop_path");
						    resourcesPath = (String)result.get("resources_path");
						    addResourcePath = (String)result.get("add_resource_path");
						    removeResourcePath = (String)result.get("remove_resource_path");
						    uploadPath = (String)result.get("upload_path");
						    bundlesPath = (String)result.get("bundles_path");
						    jobsPath = (String)result.get("jobs_path");
						} else {
					        LOGGER.severe("VCAP no credentials");
						}
					} else {
				        LOGGER.severe("VCAP no service");
					}
				} else {
			        LOGGER.severe("VCAP no services");
				}
			} else {
		        LOGGER.severe("No VCAP");
			}
        }
        catch (final Exception exception)
        {
            LOGGER.severe("Configuration EXCEPTION: " + exception.getMessage());
            exception.printStackTrace();
        }
        return result;
	}
    private static String getAPIKey() throws JSONException {
		if (apiKey == null) {
			try {
				apiKey = "Basic "
						+ DatatypeConverter.printBase64Binary(api_creds
								.getBytes("UTF-8"));
			}
			catch (final UnsupportedEncodingException exception) {
	            LOGGER.severe("Response EXCEPTION Encoding: " + exception.getMessage());
				exception.printStackTrace();
				throw new WebApplicationException(createJSONResponse(
						Status.INTERNAL_SERVER_ERROR, "Credential Encoding Exception"));
			}
		}
		return apiKey;
	}
    private static Response createJSONResponse(final Status status,
			final String text) throws JSONException {
		final JSONObject jsonObject = new JSONObject();
		jsonObject.put("description", text);

		return Response.status(status).type(MediaType.APPLICATION_JSON)
				.entity(jsonObject).build();
	}
}