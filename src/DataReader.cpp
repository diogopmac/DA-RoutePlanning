//
// Created by diogo on 11/03/2025.
//

#include <fstream>
#include <sstream>
#include "../headers/DataReader.h"

using namespace std;

DataReader::DataReader() {}

DataReader::~DataReader() {}


void DataReader::readLocations(const std::string& fileName, Graph<int>& graph) {
    ifstream file(fileName);
    if (!file) {
        cerr << "Error opening file " << fileName << endl;
        exit(1);
    }

    string name, idStr, code, parkingStr;
    int id;
    bool parking;

    string line;
    getline(file, line);

    char delimiter = ',';

    while (getline(file, line)) {
        istringstream iss(line);

        getline(iss, name, delimiter);
        getline(iss, idStr, delimiter);
        getline(iss, code, delimiter);
        getline(iss, parkingStr, '\r');

        id = stoi(idStr);
        parking = parkingStr == "1";

        cout << "name: " << name << endl;
        cout << "id: " << id << endl;
        cout << "code: " << code << endl;
        cout << "parking: " << (parking ? "Yes" : "No") << endl;
        cout << endl;

        graph.addVertex(name, id, code, parking);
    }


    cout << "File read. " << endl;
    file.close();
}

void DataReader::readDistances(const std::string& fileName, Graph<int>& graph) {
    // TODO
}




