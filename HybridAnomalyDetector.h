

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
protected:
    float threashold = 0.9;
public:
	HybridAnomalyDetector();
	~HybridAnomalyDetector() override;
    virtual void makeReport(map<string, vector<float>> map, vector<correlatedFeatures> cf, vector<AnomalyReport> &vec,
                            long timeStep) override;

    void setThreashold(float threashold);
};


#endif /* HYBRIDANOMALYDETECTOR_H_ */
