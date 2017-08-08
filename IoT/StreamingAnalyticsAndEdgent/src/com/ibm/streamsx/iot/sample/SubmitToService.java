package com.ibm.streamsx.iot.sample;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import com.ibm.streamsx.topology.Topology;
import com.ibm.streamsx.topology.context.AnalyticsServiceProperties;
import com.ibm.streamsx.topology.context.StreamsContext.Type;
import com.ibm.streamsx.topology.context.StreamsContextFactory;

public class SubmitToService {

	public static void submitTopologyToService(Topology topology) throws Exception{

		// Add credential file and Streaming Analytics service name to config 
		Map<String, Object> config = new HashMap<>();
		String credentialFile = "/path/to/vcap/file/vcap.json"; //Change this to the credentials file
		String serviceName = "service_name"; //Insert your service name here
		config.put(AnalyticsServiceProperties.VCAP_SERVICES, new File(credentialFile));
		config.put(AnalyticsServiceProperties.SERVICE_NAME, serviceName);
		// Submit application bundle to Bluemix
		StreamsContextFactory.getStreamsContext(Type.ANALYTICS_SERVICE).submit(topology, config);
	}
}
