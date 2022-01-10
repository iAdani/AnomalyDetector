// Author: Guy Adani 208642884

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};

class StandardIO  : public DefaultIO{

    // reads from the user
    virtual string read(){
        string input;
        cin >> input;
        return input;
    }

    // writes to screen
    virtual void write(string text){
        cout << text;
    }

    // writes to screen
    virtual void write(float f){
        cout << f;
    }

    // reads from the user
    virtual void read(float* f){
        cin >> *f;
    }
};


// you may edit this class
class Command{
	DefaultIO* dio;
    string description;

public:
	Command(DefaultIO* dio, string des):dio(dio){
        description = des;
    }
	virtual void execute()=0;
	virtual ~Command(){}

    // returns the command's description
    string getDesc() { return description; }

    // returns the command's IO
    DefaultIO* getIO() { return dio; }
};

//     ~ Command Classes ~

// For command 1: used to upload a timeseries csv file
class Command1 : public Command {
public:
    Command1(DefaultIO* dio) : Command(dio,  "upload a time series csv file"){ }

    virtual void execute(){
        DefaultIO* io = getIO();
        string input = "";
        ofstream train;
        ofstream test;

        // uploading to anomalyTrain.csv
        train.open("anomalyTrain.csv");
        io->write("Please upload your local train CSV file.\n");
        input = io->read();
        while (input != "done" && input != "done\n") {
            train << input;
            input = io->read();
        }
        train.close();
        io->write("Upload complete.\n");

        // uploading to anomalyTest.csv
        test.open("anomalyTest.csv");
        io->write("Please upload your local test CSV file.\n");
        input = io->read();
        while (input != "done" && input != "done\n") {
            test << input;
            input = io->read();
        }
        test.close();
        io->write("Upload complete.\n");
    }
};

// For command 2: used to set algorithm settings
class Command2 : public Command{
    HybridAnomalyDetector *detector;

    // check if the float is between 0 and 1
    bool checkThres(float t) {
        return (1 >= t && 0 <= t);
    }
public:
    Command2(DefaultIO* dio, HybridAnomalyDetector* det) : Command(dio,  "algorithm settings"){
        detector = det;
    }

    // show the current threshold and changes it
    virtual void execute(){
        DefaultIO* io = getIO();

        // show the threshold
        io->write("The current correlation threshold is ");
        io->write(detector->getThreshold());
        io->write("\n");

        // change the threshold
        io->write("Type a new threshold\n");
        float newThres = stof(io->read());
        bool isValid = checkThres(newThres);
        while (!isValid) {
            io->write("please choose a value between 0 and 1.\n");
            newThres = stof(io->read());
            isValid = checkThres(newThres);
        }
        detector->setThreshold(newThres);
    }
};

// For command 3: used to detect anomalies
class Command3 : public Command{
    vector<AnomalyReport> *report; // "shared" by commands 3,4,5
    HybridAnomalyDetector *detector;

public:
    Command3(DefaultIO* dio, vector<AnomalyReport>* rep, HybridAnomalyDetector *det) :
                                                        Command(dio,  "detect anomalies") {
    report = rep;
    detector = det;
    }

    // run the Hybrid detector on the csv files
    virtual void execute(){
        DefaultIO *io = getIO();
        report->clear();

        // run learnNormal for train input
        TimeSeries trainTs("anomalyTrain.csv");
        detector->learnNormal(trainTs);

        // run detect for test input
        TimeSeries TestTs("anomalyTest.csv");
        vector<AnomalyReport> rep = detector->detect(TestTs);

        // insert the detect result to the "shared" report
        for (auto c = rep.begin(); c != rep.end(); c++) {
            report->push_back(*c);
        }

        io->write("anomaly detection complete.\n");
    }
};

// For command 4: used to display results
class Command4 : public Command{
    vector<AnomalyReport> *report; // "shared" by commands 3,4,5

public:
    Command4(DefaultIO* dio, vector<AnomalyReport>* rep)
                             : Command(dio,  "display results"){
    report = rep;
    }

    virtual void execute(){
        DefaultIO* io = getIO();
        string line = "";

        // writing the report
        for (auto c = report->begin(); c != report->end(); c++) {
            io->write(c->timeStep);
            io->write("\t" + c->description + "\n");
        }

        io->write("Done.\n");
    }
};

// For command 5: used to upload anomalies and analyze results
class Command5 : public Command{
    vector<AnomalyReport> *report; // "shared" by commands 3,4,5
    HybridAnomalyDetector* detector;

public:
    Command5(DefaultIO* dio, vector<AnomalyReport> *rep, HybridAnomalyDetector* det)
                                        : Command(dio,  "upload anomalies and analyze results"){
        report = rep;
        detector = det;
    }

    // to check the reports in a row, the algorithm uses 2 vectors;
    // 1st for different descriptions (features) and 2nd for their length.
    virtual void execute(){
        DefaultIO* io = getIO();

        // preparing the vectors
        vector<AnomalyReport> difReports;
        vector<int> lengths;
        int repCount = 0;
        string lastDescription = "";
        for (auto c = report->begin(); c != report->end(); c++) {
            if (lastDescription.compare(c->description) == 0) {         // if there's a streak
                lengths.back()++;
            } else {
                difReports.push_back(*c);
                lengths.push_back(0);
                lastDescription = c->description;
            }
        }

        // reading the input file. splits the values to 2 vectors of first and last values
        io->write("Please upload your local anomalies file.\n");
        vector<int> begins;                             // the first detections from the file
        vector<int> ends;                              // the last detections from the file
        int commaPos = 0;                               // used to find the comma position in the input
        string input = io->read();
        while (input != "done" && input != "done\n") {
            commaPos = input.find(',');
            begins.push_back(stoi(input.substr(0, commaPos)));
            ends.push_back(stoi(input.substr(commaPos + 1, input.length() - commaPos - 2)));
            input = io->read();
        }
        io->write("Upload complete.\n");

        // analyzing the detection. names are as described in the instructions
        float P = begins.size();                        // P

        TimeSeries ts("anomalyTest.csv");
        int n = ts.getRowNum();
        int detectsSum = 0;
        for (int i = 0; i < P; i++) {
            detectsSum += (ends[i] - begins[i] + 1);
        }
        float N = n - detectsSum;                       // N

        float TP = 0;                                   // TP
        int reportsNum = difReports.size();
        int rb, re;                                         // begin and end for the reports
        for (int i = 0; i < P; i++) {                   // for each "Hariga"
            for (int j = 0; j < reportsNum; j++) {      // look for a parallel report
                rb = difReports[j].timeStep;
                re = rb + lengths[j];
                if (ends[i] >= rb && re >= begins[i]) { // found TP
                    TP++;
                    break;
                }
            }
        }

        float FP = reportsNum - TP;                     // FP

        // writing
        io->write("True Positive Rate: ");
        io->write((int)(1000.0 * TP / P) / 1000.0f);
        io->write("\nFalse Positive Rate: ");
        io->write((int)(1000.0 * FP / N) / 1000.0f);
        io->write("\n");

//        // cleaning for next time
//        difReports.clear();
//        lengths.clear();
//        begins.clear();
//        ends.clear();
    }
};

#endif /* COMMANDS_H_ */
