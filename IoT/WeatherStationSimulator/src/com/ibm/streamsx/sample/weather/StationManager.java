package com.ibm.streamsx.sample.weather;

import java.util.ArrayList;
import java.util.List;

import org.apache.edgent.function.Supplier;

/**
 * This class creates the list of simulated weather stations. 
 * Ideally, each station would send its data directly to the Watson IoT platform.
 * But because this is a simulation, we only have one device registered, so we have to send the readings for each station
 * in a round-robin format. This reporting of readings is is handled by this class.
 * It implements the Edgent <code>Supplier</code> interface, and so is polled by the Edgent topology for readings.
 * The frequency of the polling is set in WeatherStationSimulator.
 * Every time the <code>get()</code> method is called,
 *  it will return a reading for the next station in the list; starting from the beginning after all stations have reported..
 * */
public class StationManager implements Supplier<Reading> {

	private static final long serialVersionUID = -190419225260994967L;

	public static final String LOC1 = "tor";
	public static final String LOC2 = "mkm";
	private List<Station> stations;

	static final double[] toronto_lats = {43.722,43.685,43.685,43.677,43.673,43.700,43.718,43.665,43.664,43.6648,43.716,43.693,43.689,43.706,43.676,43.712,43.686,43.703,43.701,43.682};
	static final double[] toronto_lons = {-79.384,-79.474,-79.347,-79.421,-79.345,-79.294,-79.414,-79.49175,-79.425,-79.384788,-79.311,-79.298,-79.297,-79.402,-79.325,-79.520,-79.313,-79.404,-79.323,-79.476};
	static final double[] markham_lats  ={43.870,43.880,43.844750,43.8570,43.830,43.8320,43.880,43.846,43.8670,43.853330,43.870,43.880,43.850,43.870,43.830,43.850,43.880,43.846,43.860,43.840,43.910,43.880,43.850};
	static final double[] markham_lons  = {-79.271,-79.362,-79.330,-79.370,-79.310,-79.36640,-79.310,-79.350,-79.423360,-79.28040,-79.271,-79.362,-79.330,-79.370,-79.310,-79.330,-79.310,-79.350,-79.360,-79.340};


	private int ct = 0;
	public StationManager(int numStations) {
		stations = new ArrayList<Station>();
		stations.addAll(getStationsForArea(LOC1, numStations/2));
		stations.addAll(getStationsForArea(LOC2, numStations/2));

	}
	
	/**
	 * Create the specified number of stations within the given id, 
	 * @param location location id, either <code>StationManager.LOC1 or LOC2</code>
	 * @param num number of stations
	 * @return a list of stations a list containing <code>num</code> stations within the specified <code>location</code>
	 */
	public List<Station> getStationsForArea(String location, int num){
		List<Station> stationList = new ArrayList<Station>();
		
		//Decide which list of points to use depending on the specified location id. 
		double[] latitudes = LOC1.equals(location) ? toronto_lats : markham_lats;
		double[] longitudes =  LOC1.equals(location) ? toronto_lons : markham_lons;

		for (int i =0; i< num; i++){
			double lat = latitudes[i];
			double lon = longitudes[i];

			Station st = new Station(location+ "_"+(i+1), location, false,lat, lon);
			stationList.add(st);
		}
		
		
		//One station in each location is set to generate bad data
		//Choose a station to be broken
		
		int bad_index = 3;
		if (LOC1.equals(location)){
			bad_index = 7;
		} 

		Station bad = stationList.get(bad_index);
		bad.setBroken();
	//	System.out.println("Picked  " +  bad.getId() + " to be bad");
		return stationList;
	}


	@Override
	public Reading get() {
		Station st = stations.get(ct);
		ct ++;
		if (ct == stations.size()){
			ct = 0;
		}
		return st.getTemp();

	}


}