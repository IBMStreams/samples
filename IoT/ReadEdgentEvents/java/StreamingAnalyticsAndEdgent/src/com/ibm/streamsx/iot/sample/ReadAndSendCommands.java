package com.ibm.streamsx.iot.sample;
import java.util.Date;

import com.ibm.json.java.JSONObject;
import com.ibm.streamsx.iot.Device;
import com.ibm.streamsx.iot.DeviceCmd;
import com.ibm.streamsx.iot.DeviceEvent;
import com.ibm.streamsx.iot.IotStreams;
import com.ibm.streamsx.topology.TStream;
import com.ibm.streamsx.topology.Topology;

/**
 * This class demonstrates reading events from the Watson IoT platform and sending commands in response.
 * You must first submit the IotPlatformBluemix application to your Streams or Streaming Analytics instance 
 * before running this application.
 * */
public class ReadAndSendCommands {
	public static void main(String[] args) throws Exception {
		// create a Streams Topology
		Topology topology = new Topology("ReadAndSendCommands");
		//subscribe to the "sensors" events stream
		TStream<DeviceEvent> events = IotStreams.eventsSubscribe(topology, "sensors");
		events.print();

		//now send a command as  a response
		//Note that for demonstration purposes this is sending a command for every event, typically you would only send commands as needed
		TStream<DeviceCmd> command = events.transform(event -> {
			String typeId = event.getDevice().getTypeId();
			String deviceId = event.getDevice().getId();
			Device device = new Device(typeId, deviceId);
			JSONObject data = new JSONObject();
			data.put("msg",   "Alert! Show LED @ " + new Date().toString()); //$NON-NLS-1$ //$NON-NLS-2$
			return new DeviceCmd(device, "display", null, data); //$NON-NLS-1$
		});

		//publish a stream of commands
		IotStreams.commandPublish(command);
		//submit the job
		SubmitToService.submitTopologyToService(topology);
	}
}
