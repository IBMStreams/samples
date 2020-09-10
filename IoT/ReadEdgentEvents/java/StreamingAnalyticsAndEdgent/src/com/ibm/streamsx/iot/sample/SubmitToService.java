package com.ibm.streamsx.iot.sample;

import java.io.File;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Future;

import com.ibm.streamsx.topology.Topology;
import com.ibm.streamsx.topology.context.AnalyticsServiceProperties;
import com.ibm.streamsx.topology.context.StreamsContext.Type;
import com.ibm.streamsx.topology.context.StreamsContextFactory;

public class SubmitToService {

	public static void submitTopologyToService(Topology topology) throws Exception{

		// Add credential file and Streaming Analytics service name to config 
		Map<String, Object> config = new HashMap<>();
		String credentialFile = "/home/streamsadmin/Downloads/vcap.json"; //Change this to the credentials file
		String serviceName = "service name"; //Insert your service name here
		config.put(AnalyticsServiceProperties.VCAP_SERVICES, new File(credentialFile));
		config.put(AnalyticsServiceProperties.SERVICE_NAME, serviceName);
		// Submit application bundle to IBM Cloud
		Future<?> result = StreamsContextFactory.getStreamsContext(Type.ANALYTICS_SERVICE).submit(topology, config);
		//result should be a BigInteger
		System.out.println("Submitted job to service, job id = " + result.get().toString());
	}
}
