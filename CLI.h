// Author: Guy Adani 208642884

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	vector<Command*> commands;
    vector<AnomalyReport> *report;
    bool isRunning = true;
	HybridAnomalyDetector detector;

public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
	void exit();
	string createMenu();
};

#endif /* CLI_H_ */
