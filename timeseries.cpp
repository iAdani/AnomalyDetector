// Author: Guy Adani 208642884

#include "timeseries.h"
#include <string>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

// return true if the given file name exists
bool TimeSeries::fileExists(const char* name) {
    std::string str(name);
    ifstream f(str.c_str());
    return f.good();
}

// imports the CSV file
void TimeSeries::importCSV() {
    ifstream stream(this->_fileName);        // stream the file
    vector<string> columnNames;                 // the column names (map keys)
    string line;                                // current line reading
    int numOfColumns;

    getline(stream, line, '\n');   // get the first line into 'line'
    line += ",";
    int begin = 0;                              // the begin of each column name
    for (int end = 0; end < line.length(); end++) {   // get the column names to columnNames
        if (line[end] == ',') {
            columnNames.push_back(line.substr(begin, end - begin));
            begin = end + 1;
        }
    }

    numOfColumns = columnNames.size();
    for (int i = 0; i < numOfColumns; i++) {    // insert the columns to the map
        this->columns[columnNames[i]] = vector<float>();
    }
    this->colMapKeys = columnNames;
    
    int curColumn;                              // the current collumn to insert the cell
    float cellVal;                              // current cell value (for cleaner code)
    while (getline(stream, line, '\n')) {       // get the cells into the columns
        line += ",";
        begin = 0, curColumn = 0;
        for (int end = 0; end < line.length(); end++) {   
            if (line[end] == ',') {
                cellVal = stof(line.substr(begin, end - begin));
                columns[columnNames[curColumn]].push_back(cellVal);
                curColumn++;
                begin = end + 1;
            }
        }
    }
}

// prints the map. NOTICE: columns are presented as rows!
void TimeSeries::printMap() const {
    cout << "columns: " << this->colMapKeys.size() << endl;
    for (string column : this->colMapKeys) {
        cout << column;
        for (float cell : columns.at(column)) {
            cout << "\t" << cell;
        }
        cout << endl;
    }
}

// returns the column names
const vector<string> &TimeSeries::getColumnNames() const {
    return this->colMapKeys;
}

// returns the map
map<string, vector<float>> TimeSeries::getMap() const {
    return this->columns;
}

// returns the number of rows in the table (w\o names)
int TimeSeries::getRowNum() const {
    return columns.at(colMapKeys[0]).size();
}