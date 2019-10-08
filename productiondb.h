#include <vector>
#include <map>
#include <string>
#ifndef ENTRY_H
#include "entry.h"
//#include <iostream> //remove later!!!
#ifndef PRODUCTIONDB_H
#define PRODUCTIONDB_H

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
	std::string getName();
	station(std::string initName);
	~station();
};

class productiondb {
	std::map< std::string, station*> stations;
public:
	void addData(entry oneEntry);
	productiondb();
	~productiondb();
};

#endif
#endif
