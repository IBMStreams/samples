package com.ibm.streamsx.sample.weather;

import java.io.File;
import java.util.concurrent.TimeUnit;

import org.apache.edgent.connectors.iot.IotDevice;
import org.apache.edgent.connectors.iot.QoS;
import org.apache.edgent.connectors.iotp.IotpDevice;
import org.apache.edgent.providers.direct.DirectProvider;
import org.apache.edgent.providers.direct.DirectTopology;
import org.apache.edgent.topology.TStream;

import com.google.gson.JsonObject;



public class WeatherStationSimulator {



	public static void main(String[] args) throws Exception {

		//verify the right number of files was specified
		checkArgs(args);

		//Compute the path to the device config file
		String deviceCfgParam = args[0];
		String deviceCfgPath = getPathToDeviceCfg(deviceCfgParam);

		//Create and start the edgent application
		createTopo(deviceCfgPath);

		//Start the shutdown timer. The IoT platform on IBM Cloud has a limit on the amount of data that can be sent
		//if this application is left running indefinitely that could be used up quickly.
		//The optional timeout parameter is used to determine how long to run the application.
		//Default is 20 minutes
		int timeout_in_minutes = 20;
		if (args.length == 2) {
			timeout_in_minutes = Integer.parseInt(args[1]);
		}
		startShutdownTimer(timeout_in_minutes);


	}



	/***
	 * Create the Edgent topology that will send simulated data
	 * @param deviceCfg the device registered with the Watson IoT platform
	 * @param id location id
	 */
	public static void createTopo(String deviceCfg) {
		int numStations = 20;
		
		//this is the core Edgent application, 
		DirectProvider tp = new DirectProvider();

		DirectTopology topology = tp.newTopology("WeatherData");
		//create a device to connect to the IOT platform.

		IotDevice device = new IotpDevice(topology, new File(deviceCfg));
		//the StationManager serves as the data source, reporting data for each station every 500 ms.
		
		StationManager mg = new StationManager(numStations);
		int pollingFrequency = 500; //change this to report more frequently. The map will also update more rapidly.
		TStream<Reading> readingStream = device.topology().poll(mg, pollingFrequency, TimeUnit.MILLISECONDS);
		//convert the stream of readings to JSON and send
		TStream<JsonObject> stationReadingsAsJSON = toJSON(readingStream);
		//send the readings to the IoT platform with event id "weather".
		
		device.events(stationReadingsAsJSON, "weather",QoS.FIRE_AND_FORGET);
		tp.submit(topology);
	}




	/***
	 * This function converts a TStream of readings to a stream of JSONObjects so that they can
	 * be submitted to the Watson IoT platform
	 */
	public static TStream<JsonObject> toJSON(TStream<Reading> input){

		return input.map(d -> {
			JsonObject j = new JsonObject();
			j.addProperty("id", d.getStation().getId());
			j.addProperty("temp",d.getTemp());
			j.addProperty("location",d.getStation().getLocation());
			j.addProperty("lat",d.getStation().lat());
			j.addProperty("lon",d.getStation().lon());
		//	System.out.println(j);
			return j;
		});

	}


	private static String getPathToDeviceCfg(String file) {
		//Figure out if the device.cfg file they specified was an absolute or relative directory
		String localFile = System.getProperty("user.dir") + System.getProperty("file.separator") + file;
		String deviceCfg = localFile;
			
		if (new File(localFile).exists()){
			deviceCfg = localFile; //relative path
		} else if (new File(file).exists()){
			deviceCfg = file; //absolute path
		} else {
		System.err.println("File " + file + "  not found in the current working directory, or as an absolute path.");
			printUsage();
		}
		return deviceCfg;
	}

	
	private static void startShutdownTimer(final int timeout_in_minutes){
		if (timeout_in_minutes == 0){
			return;
		}
		new Thread(){
			public void run(){
				try {

					Thread.sleep(timeout_in_minutes*1000*60);
					System.out.println("Exiting after " + timeout_in_minutes + " minutes.");
					System.exit(0);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}.start();

	}


	private static void printUsage() {
		System.err.println("Proper Usage is:\njava com.ibm.streamsx.sample.weather.WeatherStationSimulator /path/to/device.cfg [run-time-in-minutes].\nSpecify the path to the device.cfg file to use to connect to the Watson IoT platform. \n"+ 
				"\nThis app is designed to run for a fixed time in order not to use up your quota on the Watson IoT platform." +"\nUse the optional run-time parameter to specify how long the application should submit data, in minutes. A value of 0 means this application will run indefinitely, the default is 20.");
		System.exit(1);
	}


	private static void checkArgs(String[] args){
		if (args.length != 1 && args.length != 2 ) {
			printUsage();
		}
		if (args.length == 2) {
			try {
				
				Integer.parseInt(args[1]);
			} catch (Exception e){
				System.err.println("Application run time in minutes must be a numeric value.");e.printStackTrace();
				printUsage();
			}
		}
	}



}