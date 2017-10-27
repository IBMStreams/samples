package com.ibm.streamsx.sample.weather;

import java.util.Random;

public class Station {
	
	public static Random random = new Random();
	private double firstReading ;
	private String id;
	private String location;
	private String name;
	private boolean broken;
	private double longitude;
	private double latitude;
	
	public Station(String id, String location, boolean broken, double lat, double lon) {
		this.id = id;
		this.location = location;
		this.broken = broken;
		firstReading  = 71.0+ (random.nextDouble() * 4.0);
		this.latitude = lat;
		this.longitude= lon;
	}
	private boolean isBad(){
		return broken;
	}
	public String getId() {
		return id;
	}
	public Reading getTemp() {

		double reading  = firstReading;
		//keep each station's value roughly around the first reading,
		//except for bad stations which will be high
		if (isBad()) {
			if (location.equals(StationManager.LOC1)){
				reading = 82.0 + (random.nextDouble() * 16.0) ;
			} else {
				reading = 55 - (random.nextDouble() * 10.5);
			}
		} else {
			reading =  reading + (1.0 * random.nextDouble());
		}
		return new Reading(reading, this);
	}
	public String getLocation() {
		return location;
	}
	public String getName() {
		return name;
	}

	public double lon(){
		return longitude;
	}
	public double lat(){
		return latitude;
	}
	public void setBroken() {
		this.broken = true;
	}




}