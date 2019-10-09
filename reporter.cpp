#include "reporter.h"
#include <iostream>
#include <string>
#include <utility>

reporter::reporter() {
}

reporter::reporter(productiondb &db) { //the only constructor because without a db to connect is useless
	//secure pointer to database
	//get list of stations to poll(?)
	connection = &db;
	db.getTables(&stationList);
}

reporter::~reporter() {
	stationList.clear();
	connection = NULL;
}

void reporter::prettyPrint(int year) {
	printf("*******%4d*******\n\n", year);
//	printf("Station: %s\n\n");
}

void reporter::printFullReport(int year) {
	//strategy:
	// for each station, get (year) function
	for(std::vector<std::string>::iterator it = stationList.begin(); it != stationList.end(); it++) {
		//call get yearly station data func
		query reportQuery(*it, year, true);
		connection->getStationMonthly(&reportQuery);
		//std::cout << reportQuery.response.size() << std::endl;
	}
}

void reporter::printStationReport(int year) {
	//strategy:
	// for each station, for each year, get all export values (station x value)
	for(std::vector<std::string>::iterator it = stationList.begin(); it != stationList.end(); it++) {
		//call get yearly station data func
		query reportQuery(*it, year, true);
		connection->getStationYearly(&reportQuery);
		//std::cout << reportQuery.response.size() << std::endl;
	}
}
