// Author: Guy Adani 208642884

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <iostream>
#include <map>
#include <vector>

using namespace std;

class TimeSeries{
	const char* _fileName;
	map<string, vector<float>> columns;
	vector<string> colMapKeys;				// column names
	
	bool fileExists(const char* name);
	void importCSV();

public:

	TimeSeries(const char* CSVfileName): _fileName(CSVfileName) {
		if (!fileExists(_fileName)) {
			cout << "file " << CSVfileName << " doesn't exist" << endl;
		} else {
			importCSV();
		}
	}


	void printMap() const;
	const vector<string> &getColumnNames() const;
	map<string, vector<float>> getMap() const;
	int getRowNum() const;
};



#endif /* TIMESERIES_H_ */
