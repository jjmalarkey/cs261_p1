GXX = g++
GXXFLAGS = -Wall -g -std=c++11

showreport: showreport.o entry.o productiondb.o reporter.o
	$(GXX) $(GXXFLAGS) -o showreport showreport.o entry.o productiondb.o reporter.o

showreport.o: showreport.cpp entry.h productiondb.h reporter.h
	$(GXX) $(GXXFLAGS) -c showreport.cpp

entry.o: entry.h

production.o: productiondb.h entry.h

reporter.o: productiondb.h entry.h

clean: 
	rm *.o
