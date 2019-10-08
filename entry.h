#ifndef ENTRY_H
#define ENTRY_H

#include <string>

class entry {
private:
	int year, month, day, amount;
	std::string station, resource;
public:
	entry(int inputYear, int inputMonth, int inputDay, std::string inputStation, std::string inputResource, int inputAmount);
	~entry();
//	void serialize();
	std::string getStation();
	std::string getResource();
	int getAmount();
	int getDay();
	int getMonth();
	int getYear();
};

#endif
