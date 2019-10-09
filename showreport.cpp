#include <iostream>
#include <fstream>
#include <string>
#include "productiondb.h"
#include "entry.h"
#include "reporter.h"

using namespace std;

int main(int argc,char* argv[])
{
    productiondb db;

    if (argc != 2) {
	cout << "Usage: " << argv[0] << " <datafile>" << endl;
	exit(0);
    }

    // Read the data

    char* datafile = argv[1];
    ifstream infile(datafile);
    int year;
    int month;
    int day;
    string station;
    string resource;
    int amount;

    while (!infile.eof()) {
	infile >> year;
	infile >> month;
	infile >> day;
	infile >> station;
	infile >> resource;
	infile >> amount;

	if (!infile.eof()) {
	    entry oneEntry(year,month,day,station,resource,amount);
	    db.addData(oneEntry);
	}
    }

    // Output the report

	reporter reporter(db);
    for (int year = 2045; year <= 2047; year++) {
	reporter.printFullReport(year);
    }

    for (int year = 2045; year <= 2047; year++) {
	reporter.printStationReport(year);
    }
	
    return(0);
}

