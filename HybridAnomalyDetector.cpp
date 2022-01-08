// Author: Guy Adani 208642884

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() { }

HybridAnomalyDetector::~HybridAnomalyDetector() { }

// decides which funcion calculates the threshold depending on correlation
float HybridAnomalyDetector::calcThres(vector<Point*> points, Line reg, Circle minCir, float cor) {
    if (cor > thresh) {
	    return maxDev(points, reg);
    }

    // else the correlation is between 0.5 and the threshold
    return minCir.radius;
}

bool HybridAnomalyDetector::detected(Point p, correlatedFeatures corFeat) {
    if (corFeat.corrlation > thresh) {
        return SimpleAnomalyDetector::detected(p, corFeat);
    }

    // else the correlation is between 0.5 and the threshold
    float distance = dist(corFeat.min_cir.center, p);
    return distance > corFeat.threshold;
}

// sets a new threshold
void HybridAnomalyDetector::setThreshold(float t){ thresh = t; }

// returns the threshold
float HybridAnomalyDetector::getThreshold(){ return thresh; }

