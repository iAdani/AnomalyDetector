// Author: Guy Adani 208642884

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include <cstdlib>

SimpleAnomalyDetector::SimpleAnomalyDetector() { }

SimpleAnomalyDetector::~SimpleAnomalyDetector() { }

// returns correlated features
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    cf.clear();
	map<string, vector<float>> tMap = ts.getMap();
	const vector<string> columns = ts.getColumnNames();

	int numOfCol = columns.size(), c;		// m, c, p are like in the pseudo 
	float m, p;
	float *col1, *col2;						// pearson needs float* not vector
	for (int i = 0; i < numOfCol; i++) {	// implementaion of the given pseudo
		m = 0.5, c = -1;
		col1 = &tMap.at(columns[i])[0];
		for (int j = i + 1; j < numOfCol; j++) {
			col2 = &(tMap.at(columns[j])[0]);
			p = abs(pearson(col1, col2, ts.getRowNum()));
			if (p > m) {
				m = p, c = j;
			}
		}
		if (c > -1) {
			string feat1 = columns[i], feat2 = columns[c];				// just names for cleaner code
			vector<Point*> pointsVec = makePointVec(feat1, feat2, ts);	// makes a vector of the points
			Point** pointsArr = &pointsVec[0];							// Point** for linear_reg
			Line reg = calcLinReg(pointsArr, pointsVec.size());
			Circle minCir = findMinCircle(pointsArr, pointsVec.size());
			float threshold = calcThres(pointsVec, reg, minCir, m) * 1.1;

			for(int i = 0; i < pointsVec.size(); i++) {					// created in makePointVec(...)
				delete pointsVec[i];
			}

			this->cf.push_back({ feat1, feat2, m, reg, minCir, threshold });
		}
	}
}

// decides which funcion calculates the threshold depending on correlation
float SimpleAnomalyDetector::calcThres(vector<Point*> points, Line reg, Circle minCir, float cor) {
	return maxDev(points, reg);
}

// returns the maximum dev from given Points and a Line
float SimpleAnomalyDetector::maxDev(vector<Point*> points, Line reg) {
	float max = 0, curDev;
	for(int i = 0; i < points.size(); i++) {
		curDev = dev(*points[i], reg);
		if(curDev > max) {
			max = curDev;
		}
	}
	return max;
}

// makes vector of the Points, x from feat1 & y from feat 2
vector<Point*> SimpleAnomalyDetector::makePointVec
				(string feat1, string feat2, const TimeSeries& ts) {
	vector<Point*> points;
	int length = ts.getRowNum();

	for(int i = 0; i < length; i++) {			// make a *Point
		float x = ts.getMap()[feat1][i];		// values from feature1
		float y = ts.getMap()[feat2][i];		// values from feature2
		points.push_back(new Point(x, y));
	}
	return points;
}

// calculates the linear_reg for 2 features
Line SimpleAnomalyDetector::calcLinReg(Point** points, int length) {

	return linear_reg(points, length);
}

// anomaly detecting
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	map<string, vector<float>> tMap = ts.getMap();
	vector<AnomalyReport> report;
	vector<float> f1, f2;									// the features values (loop)
	
	for (int i = 0; i < cf.size(); i++) {					// for every correlated features
	f1 = tMap.at(cf[i].feature1), f2 = tMap.at(cf[i].feature2);
		for (int j = 0; j < f1.size(); j++) { 
			if (detected(Point(f1[j], f2[j]), cf[i])) { // anomaly!
				string description = cf[i].feature1 + "-" + cf[i].feature2;
				report.push_back({ description, j + 1 });	// j+1 b\c time starts at 1
			}
		}
	}

	return report;
}

// detect if theres an anomaly
bool SimpleAnomalyDetector::detected(Point p, correlatedFeatures corFeat) {
	return dev(p, corFeat.lin_reg) > corFeat.threshold;
}