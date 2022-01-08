// Author: Guy Adani 208642884

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	Circle min_cir;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	vector<correlatedFeatures> cf;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

	vector<Point*> makePointVec(string feat1, string feat2, const TimeSeries& ts);
	Line calcLinReg(Point** points, int length);
	float maxDev(vector<Point*> points, Line reg);
	float maxCor();
	virtual float calcThres(vector<Point*> points, Line reg, Circle minCir, float cor);
	virtual bool detected(Point p, correlatedFeatures corFeat);

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
