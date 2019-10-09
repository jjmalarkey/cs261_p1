#ifndef PRODUCTIONDB_H
#include "productiondb.h"
#endif
#ifndef REPORTER_H
#define REPORTER_H
#include <vector>

class reporter {
	productiondb * connection;
	std::vector<std::string> stationList;
	void prettyPrint(int year);
public:
	void printFullReport(int year);
	void printStationReport(int year);
	reporter();
	reporter(productiondb &db);
	~reporter();
};

#endif
