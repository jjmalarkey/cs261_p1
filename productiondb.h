#include <vector> 
#include <map>
#include <string>
#include <utility>
#ifndef ENTRY_H
#include "entry.h"
#endif
//#include <iostream> //remove later!!!
#ifndef PRODUCTIONDB_H
#define PRODUCTIONDB_H

struct query { //to attempt to safely convery data to requesting services
	std::string station;
	int year;
	std::vector<std::map<std::string, int>> response;
	bool pass;
	query(std::string initStation, int initYear);
	~query();
};

class resourceAmount {
	std::string name;
	int amount;
public:
	std::string getName();
	int getAmount();
	void setAmount(int newAmount);
	resourceAmount(entry *oneEntry);
	~resourceAmount();
};

class dailyRecord {
	int day;
	//std::vector< resourceAmount > intake;
	std::map< std::string, resourceAmount*> intake;
public:
	void addResource(entry *oneEntry);
	int getResourceAmount(std::string resource);
	int getDay();
	dailyRecord(int day);
	~dailyRecord();
};

class monthlyRecord {
	std::vector< dailyRecord* > intake;
	std::map<std::string, int> total;
public:
	void addResource(entry *oneEntry);
	int getResourceAmount(std::string resource);
	void getAll(query * reportQuery);
	//void updateMonthlyRecord();
	monthlyRecord();
	~monthlyRecord();
};

class yearlyRecord {
	std::map< std::string, int> total;
	monthlyRecord * intake[12];
	int year;
public:
	void addResource(entry *oneEntry);
	int getResourceAmount(std::string resource);
	int getYear();
	void getAll(query * reportQuery);
	void getMonthly(query * reportQuery);
	yearlyRecord(int initYear);
	~yearlyRecord();
};

class station {
	std::map< std::string, int> total;
	std::vector< yearlyRecord*> intake;
	std::string name;
public:
	void addResource(entry *oneEntry);
	int getResourceAmount(std::string resource);
	void getYearly(query * reportQuery);
	void getMonthly(query * reportQuery);
	std::string getName();
	station(std::string initName);
	~station();
};

class productiondb {
	std::map< std::string, station*> stations;
public:
	int getTables(std::vector<std::string> * ptr);
	void addData(entry oneEntry);
	void getStationMonthly(query * reportQuery);
	void getStationYearly(query * reportQuery);
	productiondb();
	~productiondb();
};

#endif
