package com.ibm.streamsx.iot.sample;
import java.io.File;
import java.util.HashMap;
import java.util.Map;

import com.ibm.streamsx.iot.DeviceEvent;
import com.ibm.streamsx.iot.IotStreams;
import com.ibm.streamsx.topology.TStream;
import com.ibm.streamsx.topology.Topology;
import com.ibm.streamsx.topology.context.AnalyticsServiceProperties;
import com.ibm.streamsx.topology.context.StreamsContext.Type;
import com.ibm.streamsx.topology.context.StreamsContextFactory;

/**
 * This class demonstrates reading events from the Watson IoT platform.
 * You must first submit the IotPlatformBluemix application to your Streams or Streaming Analytics instance 
 * before running this application.
 * */
public class ReadFromWatsonIoT {
	@SuppressWarnings("serial")
	public static void main(String[] args) throws Exception {
		// create a Streams Topology
		Topology topology = new Topology("ReadFromIoT");
		//subscribe to the "sensors" events stream
		TStream<DeviceEvent> events = IotStreams.eventsSubscribe(topology, "sensors");
		events.print();
		// Add credential file and Streaming Analytics service name to config 
		Map<String, Object> config = new HashMap<>();
		String credentialFile = "/home/streamsadmin/Downloads/vcap.json"; //Change this to the credentials file
		config.put(AnalyticsServiceProperties.VCAP_SERVICES, new File(credentialFile));
		String serviceName = "sa_nd"; //Insert your service name here
		config.put(AnalyticsServiceProperties.SERVICE_NAME, serviceName);
		// Submit application bundle to Bluemix
		StreamsContextFactory.getStreamsContext(Type.ANALYTICS_SERVICE).submit(topology, config);
	}
}
