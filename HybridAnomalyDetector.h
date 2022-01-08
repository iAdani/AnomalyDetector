// Author: Guy Adani 208642884

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
	float thresh = 0.9f;
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual float calcThres(vector<Point*> points, Line reg, Circle minCir, float cor);
	virtual bool detected(Point p, correlatedFeatures corFeat);

	void setThreshold(float t); // sets a new threshold
	float getThreshold(); // returns the threshold

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
