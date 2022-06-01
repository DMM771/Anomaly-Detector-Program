
#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub bl

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

vector<Point> getPoints(float *a, float *b, int size){
    //vector<Point> points = *listPoints;
    vector<Point> listPoints;
    (listPoints).reserve(size);
for (int i =0; i < size; i++){
        listPoints.emplace_back(a[i], b[i]);
    }
    return listPoints;
}

/******
* Function Name: learnNormal
* Input: const TimeSeries& ts - time series.
* Output: void.
* The function gets a time series and then check every couple of columns if there is a correlation between
 * them, if so it saves for every collumn the collumn that is most correlated to. the function fills
 * the correlated features vector cf.
******/

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    float lineTreshold =0.9;
    float minCorrelation = 0.5;
    float * aNonConst;
    float * bNonConst;
    float *finalConsA;
    float *finalConsB;
    int size;
    string feature1,feature2;
    vector<Point> listOfPoints;


    for(int i = 0; i < ts.result.size(); i++){
        feature1 = ts.result[i].first;
        float m = 0;
        int c = -1;
        for(int j = i + 1; j < ts.result.size(); j++){
            float const *a = ts.result[i].second.data();
            aNonConst = const_cast<float *>(a);
            float const *b = ts.result[j].second.data();
            bNonConst = const_cast<float *>(b);
            size = min(ts.result[i].second.size(), ts.result[j].second.size());
            //get absolute value from pearson
            float p = abs(pearson(aNonConst, bNonConst, size));
            if(p > m){
                finalConsA = aNonConst;
                finalConsB = bNonConst;
                //set local feature2 to correct name
                feature2 = ts.result[j].first;
                m = p;
                c = j;
            }
        }
        // if c != -1, correlation has been found, associate
        if (c != -1 && m >= minCorrelation){

            correlatedFeatures cor = correlatedFeatures();
            listOfPoints = getPoints(finalConsA, finalConsB, size);

            //create new correlation and update fields
            //cor = new correlatedFeatures;
            cor.feature1 = feature1;
            cor.feature2 = feature2;



            Point **p = new Point *[size];
            for (int t = 0; t < size; ++t) {
                p[t] = new Point(listOfPoints.at(t).x, listOfPoints.at(t).y);
            }

            cor.lin_reg = linear_reg(p, size);
            cor.corrlation = m;
            float maxDev = 0;
            for(int l = 0; l < size; l++){
                float temDev = dev(listOfPoints[l], cor.lin_reg);
                if (temDev > maxDev){
                    maxDev = temDev;

                }
            }
            cor.threshold = (float) 1.1 * maxDev ;
            if(m < 0.9) {
               cor.threshold = 1.1 * findMinCircle(p, size).radius;
               cor.minCircle = findMinCircle(p, size);

            }
            cf.push_back(cor);
        }
    }

}

/******
* Function Name: detect
* Input: const TimeSeries& ts - time series.
* Output: void.
* The function detects anomalies using the makeReport method and returns a vector of reports.
******/

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> reportVec;
    for(int i = 0; i < ts.result[0].second.size(); ++i){\
    makeReport(ts.mp, cf, reportVec, (long)i);
    }
    return reportVec;
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
void SimpleAnomalyDetector::makeReport( map<string,vector<float>> map, vector<correlatedFeatures> cf, vector<AnomalyReport> &vec, long timeStep)
{
    string feature1, feature2;

    for (correlatedFeatures c:cf){
        feature1 = c.feature1;
        feature2 = c.feature2;
        Point p = Point(map.at(feature1).at(timeStep),map.at(feature2).at(timeStep));
        if(dev(p, c.lin_reg) > c.threshold){
            const string description = feature1 + "-" + feature2;
            AnomalyReport ar(description, timeStep + 1);
            vec.push_back(ar);
        }
    }
}
void SimpleAnomalyDetector::setThreshold(float currentCorr){
    lineTreshold = currentCorr;
}
float SimpleAnomalyDetector::getThreshold() const{
    return lineTreshold;
}
