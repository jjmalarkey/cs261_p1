#include "reporter.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>

reporter::reporter() {
}

reporter::reporter(productiondb &db) { //the only constructor because without a db to connect is useless
	connection = &db;
	db.getTables(&stationList);
}

reporter::~reporter() { stationList.clear(); connection = NULL;
}

void reporter::prettyPrintFull(query * reportQuery) {
	//strategy: do no sorting - the response data and order should already be alphabetized
	//header format print:
	std::string calendar[13] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "Tot"};
	std::cout << "Station: " << reportQuery->station << "\n\n";
	std::cout.width(19);
	for(int i = 0; i < 13; i++) {
		std::cout << std::right << calendar[i];
		std::cout.width(8);
	}
	std::cout << std::endl << std::left;
	std::map<std::string, int>::iterator dataPtr;
	//table body:
	for(std::map<std::string, int>::iterator resIt = reportQuery->response[12].begin(); resIt != reportQuery->response[12].end(); resIt++) {
		std::cout << std::left;
		std::cout.width(11);
		std::cout << resIt->first;
		for(std::vector<std::map<std::string, int>>::iterator station = reportQuery->response.begin(); station != reportQuery->response.end(); station++) {
			dataPtr = station->find(resIt->first);
			std::cout.width(8);
			std::cout << std::right << dataPtr->second;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void reporter::prettyPrintStation(query * reportQuery, std::map<std::string, int > * resources) {
	//strategy: use fixed order of station list to order response table
	//header format print:
	printf("*******%4d*******\n", reportQuery->year);
	//table
	std::vector<int> stationTotal(stationList.size(), 0);
	std::vector<int>::iterator stationTotalIt;
	std::cout.width(22);
	for(std::vector<std::string>::iterator station = stationList.begin(); station != stationList.end(); station++) {
		std::cout << std::right << *station;
		std::cout.width(11);
	}
	std::cout << std::endl << std::left;
	std::map<std::string, int>::iterator dataPtr;
	//data
	for(std::map<std::string, int>::iterator resIt = resources->begin(); resIt != resources->end(); resIt++) {
		stationTotalIt = stationTotal.begin();
		std::cout << resIt->first;
		std::cout.width(11);
		for(std::vector<std::map<std::string, int>>::iterator station = reportQuery->response.begin(); station != reportQuery->response.end(); station++) {
			dataPtr = station->find(resIt->first);
			std::cout << std::right << dataPtr->second;
			std::cout.width(11);
			*stationTotalIt += dataPtr->second;
			stationTotalIt++;
		}
		std::cout << std::left << std::endl;
	}
	std::cout << "      Total" << std::right;
	std::cout.width(11);
	for(stationTotalIt = stationTotal.begin(); stationTotalIt != stationTotal.end(); stationTotalIt++) {
		std::cout << std::right << *stationTotalIt;
		std::cout.width(11);
	}
	std::cout << std::endl << std::endl;
}

void reporter::printFullReport(int year) {
	printf("*******%4d*******\n\n", year);
	for(std::vector<std::string>::iterator it = stationList.begin(); it != stationList.end(); it++) {
		query reportQuery(*it, year);
		connection->getStationMonthly(&reportQuery);
		prettyPrintFull(&reportQuery);
	}
}

void reporter::printStationReport(int year) {
	query totalReport("NONE", year);
	//query subQuery("none", year);
	std::map<std::string, int> resources;
	for(std::vector<std::string>::iterator it = stationList.begin(); it != stationList.end(); it++) {
		query subQuery(*it, year);
//		subQuery.station = *it;
		connection->getStationYearly(&subQuery);
		for(std::map<std::string, int>::iterator it = subQuery.response[0].begin(); it != subQuery.response[0].end(); it++) {
			std::map<std::string, int>::iterator mit = resources.find(it->first);
			if(mit == resources.end())
				resources[it->first] = 1;
		}
		totalReport.response.push_back(subQuery.response[0]);
	}
	/*
	for( auto x : resources ) {
		std::cout << x.first << "\t";
	}
	*/
	//std::cout << "\n";
	//populate entries with no resource count
	for(std::vector<std::map<std::string, int>>::iterator vit = totalReport.response.begin(); vit != totalReport.response.end(); vit++) {
//	for(std::vector<std::map<std::string, int>>::iterator vit = subQuery.response.begin(); vit != subQuery.response.end(); vit++) {
		for(std::map<std::string, int>::iterator mit = resources.begin(); mit != resources.end(); mit++) {
			std::map<std::string, int>::iterator it = vit->find(mit->first);
			if(it == vit->end())
				(*vit)[mit->first] = 0;
		}
	}
	//prettyPrintStation(&subQuery, &resources);
	prettyPrintStation(&totalReport, &resources);
}
