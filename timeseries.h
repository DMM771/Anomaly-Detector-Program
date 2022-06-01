//324680438
//313306367
/*************
* Nitzan Fisher & David Monheit
* ex2
* ************/
#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include "iostream"
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

class TimeSeries{
public:
    //vector and a map to hold the values of the time series.
    vector<pair<string,vector<float>>> result;
    map<string,vector<float>> mp;
public:
    TimeSeries(const char* CSVfileName);
    int getNumOfValues();
};
#endif /* TIMESERIES_H_ */
