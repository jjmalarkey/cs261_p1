#ifndef PRODUCTIONDB_H
#include "productiondb.h"
#endif
#ifndef REPORTER_H
#define REPORTER_H
#include <map>
#include <vector>

class reporter {
	productiondb * connection;
	std::vector<std::string> stationList;
	void prettyPrintFull(query * reportQuery);
	void prettyPrintStation(query * reportQuery, std::map<std::string, int> * resources);
public:
	void printFullReport(int year);
	void printStationReport(int year);
	reporter();
	reporter(productiondb &db);
	~reporter();
};

#endif
