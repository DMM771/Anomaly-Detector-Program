
#include "HybridAnomalyDetector.h"
HybridAnomalyDetector::HybridAnomalyDetector() {
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
}

/******
* Function Name: distance
* Input: Point a, Point b - two points.
* Output: float.
* the function calculates the distance between two points.
******/

float distance(Point a,Point b)
{
    return sqrt(pow(a.x - b.x, 2)
                + pow(a.y - b.y, 2));
}

/******
* Function Name: makeReport
* Input: map<string,vector<float>>(the map of the columns from the time series), vector<correlatedFeatures>,
 * vector<AnomalyReport> (reports to fill), long timeStep).
* Output: void.
* The function checks for every line in the time series if there are anomalies according to the
 * correlated features that found in the previous step. the function check for every couple data (according to
 * the couple that are in the correlated features vector) if the point that they are making is too far
 * from the linear reg of the correlated feature they belong to.
 * from the linear reg of the correlated feature they belong to..
******/

void HybridAnomalyDetector::makeReport(map<string, vector<float>> map, vector<correlatedFeatures> cf, vector<AnomalyReport> &vec,
                long timeStep){
    for (correlatedFeatures c:cf) {
        string feature1 = c.feature1;
        string feature2 = c.feature2;
        Point p = Point(map.at(feature1).at(timeStep),map.at(feature2).at(timeStep));
        if(c.corrlation > threashold){
            if(dev(p, c.lin_reg) > c.threshold){
                const string description = feature1 + "-" + feature2;
                AnomalyReport ar(description, timeStep + 1);
                vec.push_back(ar);
                }
            }
        if(c.corrlation <= threashold){
                if(distance(p, c.minCircle.center) > c.threshold){
                    const string description = feature1 + "-" + feature2;
                    AnomalyReport ar(description, timeStep + 1);
                    vec.push_back(ar);
            }
        }
    }
}
void HybridAnomalyDetector::setThreashold(float t)
{
    threashold = t;
}


