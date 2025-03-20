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

    while (getline(file, line)) {
        istringstream iss(line);

        getline(iss, name, ',');
        getline(iss, idStr, ',');
        getline(iss, code, ',');
        getline(iss, parkingStr, '\r');

        id = stoi(idStr);
        parking = parkingStr == "1";

        graph.addVertex(name, id, code, parking);
    }

    cout << "File read. " << endl;
    file.close();
}

void DataReader::readDistances(const std::string& fileName, Graph<int>& graph) {
    ifstream file(fileName);
    if (!file) {
        cerr << "Error opening file " << fileName << endl;
        exit(1);
    }

    string location1, location2, drivingStr, walkingStr;
    double driving, walking;

    string line;
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        getline(iss, location1, ',');
        getline(iss, location2, ',');
        getline(iss, drivingStr, ',');
        getline(iss, walkingStr, '\r');

        if (drivingStr != "X") {
            driving = stod(drivingStr);
            if(!graph.addBidirectionalEdge(location1, location2, driving, "drivable")) exit(1);
        }
        if (walkingStr != "X") {
            walking = stod(walkingStr);
            if(!graph.addBidirectionalEdge(location2, location1, walking, "walkable")) exit(1);
        }

    }
    cout << "File read. " << endl;
    file.close();
}




