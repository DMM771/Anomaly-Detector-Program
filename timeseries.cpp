//324680438
//313306367
/*************
* Nitzan Fisher & David Monheit
* ex2
* ************/
#include <map>
#include "timeseries.h"
/******
* Function Name: TimeSreies constructor
* Input: const char* CSVfileName - a CSV file.
* Output: void.
* The function gets a csv file and builds from it the vector of pairs that the time series holds ("result").
 * the pairs contains the columns of the table so the string is the collumn name and the vector holds the
 * values in the column.
******/
TimeSeries::TimeSeries(const char* CSVfileName){

    //open stream for file
    ifstream infoFile(CSVfileName);
    //set row, colName and val for use in iteration
    string row, colName;
    float val;
    //check that file has opened correctly
    if(infoFile.good()){
        //create string stream of line
        getline(infoFile, row);
        stringstream ss(row);
        //loop to get information seperated by commas
        while(getline(ss, colName, ',')){
            // set <colName, float vector> pairs to result
            result.push_back({colName,vector<float> {}});
        }
    }
    // loop to fill remaining rows with information
    while (getline(infoFile, row)){
        stringstream ss(row);
        //start at first column
        int column = 0;
        //loop to store each value in val
        while (ss >> val){

            //finally, add to second pair of vector
            result.at(column).second.push_back(val);

            //ignore ','
            if(ss.peek() == ',') ss.ignore();

            column++;
        }
    }
    for (int i = 0; i < result.size(); i++) {
        string s = result[i].first;
        vector<float> v = result[i].second;
            mp.insert(pair<string, vector<float>>(s, v));
        }

    infoFile.close();
}

int getNumOfValues() {

}