#include "productiondb.h"

//resourceAmount code 
resourceAmount::resourceAmount(entry *oneEntry) {
	amount = oneEntry->getAmount();
	name = oneEntry->getResource();
	//std::cout << "resourceAmount created\n";
}

resourceAmount::~resourceAmount() {
	//std::cout << "resourceAmount deleted\n";
}

int resourceAmount::getAmount() {
	return amount;
}

void resourceAmount::setAmount(int newAmount) {
	amount = newAmount;
}

std::string resourceAmount::getName() {
	return name;
}

//dailyRecord code

dailyRecord::dailyRecord(int initDay) {
	day = initDay;
	//std::cout << "dailyRecord created\n";
}

dailyRecord::~dailyRecord() {
	std::map< std::string, resourceAmount*>::iterator it;
	for(it = intake.begin(); it != intake.end(); it++) {
		delete (it->second);
		it->second = NULL;
	}
	intake.clear();
	//std::cout << "dailyRecord deleted\n";
}

void dailyRecord::addResource(entry *oneEntry) {
	std::string resourceName = oneEntry->getResource();
	std::map< std::string, resourceAmount*>::iterator it = intake.find(resourceName);
	if(it == intake.end()) { //if the element is new, create new resourceAmount
		intake[resourceName] = new resourceAmount(oneEntry);
	} else {// if the element exists, sum the old and new resourceAmount
		int newTotal = (oneEntry->getAmount() + it->second->getAmount());
		it->second->setAmount(newTotal);
	}
}

int dailyRecord::getResourceAmount(std::string resource) {
	int amount = 0;
	std::map<std::string, resourceAmount*>::iterator it = intake.find(resource);
	if(it != intake.end()) {
		amount = it->second->getAmount();
	}
	return amount;
}

int dailyRecord::getDay() {
	return day;
}

//monthlyRecord code
monthlyRecord::monthlyRecord() {
	//std::cout << "monthlyRecord created\n";
}

monthlyRecord::~monthlyRecord() {
	for (dailyRecord* ptr : intake) {
		delete ptr;
	}
	intake.clear();
	total.clear();
	//std::cout << "monthlyRecord deleted\n";
}

void monthlyRecord::addResource(entry *oneEntry) {
	int day = oneEntry->getDay();
	int amount = oneEntry->getAmount();
	std::string resource = oneEntry->getResource();
	std::vector<dailyRecord*>::iterator it, emplace;
	for (it = intake.begin(), emplace = intake.begin(); it != intake.end(); it++) {
		if((*it)->getDay() == day)
			break;
		if((*emplace)->getDay() < day)
			emplace++;
	}	
	if(it == intake.end()) {
		//intake.push_back(new dailyRecord(day));
		//it--;
		it = intake.insert(emplace, new dailyRecord(day));
	}
	std::map<std::string, int>::iterator mit = total.find(resource);
	if(mit != total.end()) {
		total[resource] += amount;
	} else {
		total[resource] = amount;
	}
	(*it)->addResource(oneEntry);
}

int monthlyRecord::getResourceAmount(std::string resource) {
	int totalAmount = 0;
	std::map<std::string, int>::iterator it = total.find(resource);
	if(it != total.end()) {
		totalAmount = it->second;
	}
	return totalAmount;
}

void monthlyRecord::getAll(query * reportQuery) {
	std::vector<std::pair<std::string, int>> totalExport;
	for(std::map<std::string, int>::iterator mit = total.begin(); mit != total.end(); mit++) {
		totalExport.push_back(std::make_pair(mit->first, mit->second));
	}
	reportQuery->response.push_back(totalExport);
}

//yearlyRecord code
yearlyRecord::yearlyRecord(int initYear) {
	year = initYear;
	for(int i = 0; i < 12; i++) {
		intake[i] = NULL;
	}
	//std::cout << "yearlyRecord created\n";
}

yearlyRecord::~yearlyRecord() {
	//delete all array entries
	for(int i = 0; i < 12; i++) {
		if (intake[i] != NULL) {
			delete intake[i];
			intake[i] = NULL;
		}
	}
	total.clear();
	//std::cout << "yearlyRecord deleted\n";
}

void yearlyRecord::addResource(entry *oneEntry) {
	int month = oneEntry->getMonth();
	std::string resource = oneEntry->getResource();
	int amount = oneEntry->getAmount();
	if(intake[month-1] == NULL) {
		intake[month-1] = new monthlyRecord;
	}
	std::map<std::string, int>::iterator it = total.find(resource);
	if(it != total.end()) {
		total[resource] += amount;
	} else {
		total[resource] = amount;
	}
	intake[month-1]->addResource(oneEntry);
}

int yearlyRecord::getResourceAmount(std::string resource) {
	int totalAmount = 0;
	std::map<std::string, int>::iterator it = total.find(resource);
	if(it != total.end()) {
		totalAmount = it->second;
	}
	return totalAmount;
}

int yearlyRecord::getYear() {
	return year;
}

void yearlyRecord::getAll(query * reportQuery) {
	std::vector<std::pair<std::string, int>> totalExport;
	for(std::map< std::string, int>::iterator mit = total.begin(); mit != total.end(); mit++) {
		totalExport.push_back(std::make_pair(mit->first, mit->second));
	}
	reportQuery->response.push_back(totalExport);
	reportQuery->isTwoD = true;
	reportQuery->pass = true;
}

void yearlyRecord::getMonthly(query * reportQuery) {
	for(int i = 0; i < 12; i++) {
		if (intake[i] != NULL)
			intake[i]->getAll(reportQuery);
		else { // if no data for the month, insert an empty row
			std::vector<std::pair<std::string, int>> totalExport;
			totalExport.push_back(std::make_pair("EMPTY_MONTH", i));
			reportQuery->response.push_back(totalExport);
		}
	}
	std::vector<std::pair<std::string, int>> totalExport;
	for(std::map< std::string, int>::iterator mit = total.begin(); mit != total.end(); mit++) {
		totalExport.push_back(std::make_pair(mit->first, mit->second));
	}
	reportQuery->isTwoD = false;
	reportQuery->response.push_back(totalExport);
}

//station code

station::station(std::string initName){
	name = initName;
	//std::cout << "station created\n";
}

station::~station(){
	for (yearlyRecord* ptr : intake) {
		delete ptr;
	}
	intake.clear();
	total.clear();
	//std::cout << "station deleted\n";
}

void station::addResource(entry *oneEntry) {
	int year = oneEntry->getYear();
	int amount = oneEntry->getAmount();
	std::string resource = oneEntry->getResource();
	std::vector<yearlyRecord*>::iterator it, emplace;
	for (it = intake.begin(), emplace = intake.begin(); it != intake.end(); it++) {
		if((*it)->getYear() == year) 
			break;
		if((*emplace)->getYear() < year)
			emplace++;
	}	
	if(it == intake.end()) {
		it = intake.insert(emplace, new yearlyRecord(year));
	}
	std::map<std::string, int>::iterator mit = total.find(resource);
	if(mit != total.end()) {
		total[resource] += amount;
	} else {
		total[resource] = amount;
	}
	(*it)->addResource(oneEntry);
}

int station::getResourceAmount(std::string resource) {
	int totalAmount = 0;
	std::map<std::string, int>::iterator it = total.find(resource);
	if(it != total.end()) {
		totalAmount = it->second;
	}
	return totalAmount;
}

std::string station::getName() {
	return name;
}

void station::getYearly(query * reportQuery) {
	std::vector< yearlyRecord*>::iterator it;
	for(it = intake.begin(); it != intake.end(); it++) {
		if ((*it)->getYear() == reportQuery->year)
			break;
	}
	if(it != intake.end()) {
		(*it)->getAll(reportQuery);
	} else {
		reportQuery->pass = false;
	}
}

void station::getMonthly(query * reportQuery) {
	std::vector< yearlyRecord*>::iterator it;
	for(it = intake.begin(); it != intake.end(); it++) {
		if ((*it)->getYear() == reportQuery->year)
			break;
	}
	if(it != intake.end()) {
		(*it)->getMonthly(reportQuery);
	} else {
		reportQuery->pass = false;
	}
}

//query code

query::query(std::string initStation, int initYear, bool resourceDriven) {
	station = initStation;
	year = initYear;
	isResourceDriven = resourceDriven;
}

query::~query() {
	response.clear();
}

//productiondb code

productiondb::productiondb() {

}

productiondb::~productiondb() {
	std::map<std::string, station*>::iterator mit;
	for(mit = stations.begin(); mit != stations.end(); mit++) {
		delete mit->second;
	}
	stations.clear();
}

void productiondb::addData(entry oneEntry) {
	//does the station exist?
	//if yes, proceed as normal
	//if no, find the right place for the station to be added (alphabetically)
	std::string stationName = oneEntry.getStation();
	std::map<std::string, station*>::iterator mit = stations.find(stationName);
	if(mit != stations.end()) {
		mit->second->addResource(&oneEntry);
	} else { //TODO: sort on insertion will make the map automatically alphabetized
		//intake[station] = new station(stationName);
		auto x = (stations.emplace(stationName, new station(stationName)));
		mit = x.first;
//		for (mit = stations.begin(); mit != stations.end(); mit++) {
//			if(mit->first.compare(stationName) < 0) {
//				break;
//			}
//		}
//		if (mit-> != stations.begin()
//		
	}
}

int productiondb::getTables(std::vector<std::string> * ptr) {
	std::map<std::string, station*>::iterator mit;
	for(mit = stations.begin(); mit != stations.end(); mit++) {
		ptr->push_back(mit->first);
	}
	return 0;
}

void productiondb::getStationMonthly(query * reportQuery) {
	std::map<std::string, station*>::iterator mit = stations.find(reportQuery->station);
	if(mit != stations.end()) {
		mit->second->getMonthly(reportQuery);
	} else {
		reportQuery->pass = false;
	}
}

void productiondb::getStationYearly(query * reportQuery) {
	std::map<std::string, station*>::iterator mit = stations.find(reportQuery->station);
	if(mit != stations.end()) {
		mit->second->getYearly(reportQuery);
	} else {
		reportQuery->pass = false;
	}
}
