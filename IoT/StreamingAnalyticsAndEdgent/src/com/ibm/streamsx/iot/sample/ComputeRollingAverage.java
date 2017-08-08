package com.ibm.streamsx.iot.sample;
import java.util.List;
import java.util.OptionalDouble;
import java.util.concurrent.TimeUnit;

import com.ibm.json.java.JSONObject;
import com.ibm.streamsx.iot.DeviceEvent;
import com.ibm.streamsx.iot.IotStreams;
import com.ibm.streamsx.topology.TStream;
import com.ibm.streamsx.topology.Topology;
import com.ibm.streamsx.topology.function.Function;

/**
 * This class demonstrates doing a bit more with the events received from your Edgent application.
 * It computes the rolling average in a 24 hour window of the minimum readings.
 * It also shows how to access the different attributes of the data sent from Edgent.
 * You must first submit the IotPlatformBluemix application to your Streams or Streaming Analytics instance 
 * before running this application.
 * */
public class ComputeRollingAverage {
	@SuppressWarnings("serial")
	public static void main(String[] args) throws Exception {
		// create a Streams Topology
		Topology topology = new Topology("ComputeRollingAverage");
		//subscribe to the "sensors" events stream
		TStream<DeviceEvent> events = IotStreams.eventsSubscribe(topology, "sensors");
		// We just want the payload reading which is of form
		// "reading":{"N":10,"MIN":20.02074668027724,"MAX":38.910292772562215,"MEAN":27.591084814851353,"STDDEV":6.66598501958506} 
		TStream <JSONObject>reading = events.transform(event -> ((JSONObject)(event.getData().get("reading")))); 

		TStream<Double> sensorMin = reading.transform(v -> (Double) v.get("MIN"));
		Function<List<Double>, Double> aggFn = new Function<List<Double>, Double>() { 
			double rollingAvg = 0; 
			@Override 
			public Double apply(List<Double> mins) {
				// Calculate average based on window values
				OptionalDouble optRollingAvg = mins.stream().mapToDouble( d -> d).average();
				if (optRollingAvg.isPresent()){
					rollingAvg = optRollingAvg.getAsDouble();
				}
				return rollingAvg; 
			} 
		}; 
		//Below we're doing a 30 second average so you can see data printed
		//for a 24 hour rolling average  you would use the line below that is commented out
//		TStream<Double> rollingAverageStream = sensorMin.last(24, TimeUnit.HOURS).aggregate(aggFn);
		TStream<Double> rollingAverageStream = sensorMin.last(30, TimeUnit.SECONDS).aggregate(aggFn);
		rollingAverageStream.print();
		
		SubmitToService.submitTopologyToService(topology);
	}
}
