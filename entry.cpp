#include "entry.h"
#include <string>

entry::entry(int inputYear, int inputMonth, int inputDay, std::string inputStation, std::string inputResource, int inputAmount) {
	year = inputYear;
	month = inputMonth;
	day = inputDay;
	station = inputStation;
	resource = inputResource;
	amount = inputAmount;
}

entry::~entry() {
	
}

std::string entry::getStation() {
	return station;
}

std::string entry::getResource() {
	return resource;
}

int entry::getYear() {
	return year;
}

int entry::getDay() {
	return day;
}

int entry::getMonth() {
	return month;
}

int entry::getAmount() {
	return amount;
}
/*
void entry::serialize() {
	return year + ':' + month + ':' + day + ':' + station + ':' + resource + ':' + amount + ";";
}
*/
