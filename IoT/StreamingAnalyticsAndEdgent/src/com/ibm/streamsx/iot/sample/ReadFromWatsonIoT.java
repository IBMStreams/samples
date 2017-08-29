package com.ibm.streamsx.iot.sample;
import com.ibm.streamsx.iot.DeviceEvent;
import com.ibm.streamsx.iot.IotStreams;
import com.ibm.streamsx.topology.TStream;
import com.ibm.streamsx.topology.Topology;

/**
 * This class demonstrates reading events from the Watson IoT platform.
 * You must first submit the IotPlatformBluemix application to your Streams or Streaming Analytics instance 
 * before running this application.
 * */
public class ReadFromWatsonIoT {
	public static void main(String[] args) throws Exception {
		// create a Streams Topology
		Topology topology = new Topology("ReadFromIoT");
		//subscribe to the "sensors" events stream
		TStream<DeviceEvent> events = IotStreams.eventsSubscribe(topology, "sensors");
		//The events stream contains metadata about the event, including device type and id
		//use event.getData() to extract the payload which contains the data we are interested in

		TStream <JSONObject>reading = events.transform(event -> ((JSONObject)(event.getData()))); 
		reading.print();
		SubmitToService.submitTopologyToService(topology);
	}
}
