package com.ibm.streamsx.sample.weather;

public class Reading {

	private Station station;

	public Reading(double temp, Station station) {
		this.station = station;
		this.temp = temp;
	}
	double temp;

	double getTemp(){
		return temp;
	}
	public Station getStation() {
		return station;
	}
	@Override
	public String toString() {
		return station.getId()  + " " + temp + " "+ station.getLocation();
	}

}