//
// Created by diogo on 11/03/2025.
//

#include <fstream>
#include <sstream>
#include "../headers/DataReader.h"

using namespace std;

/**
 * Constructor for the DataReader class.
 */
DataReader::DataReader() {}

/**
 * Destructor for the DataReader class.
 */
DataReader::~DataReader() {}

/**
 * Reads location data from a file and populates the graph.
 * @param fileName Path to the file containing location data.
 * @param graph Reference to the graph to populate.
 */
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

    file.close();
}

/**
 * Reads distance data from a file and populates the graph with edges.
 * @param fileName Path to the file containing distance data.
 * @param graph Reference to the graph to populate.
 */
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
            if(!graph.addBidirectionalEdge(location1, location2, driving, "driving")) exit(1);
        }
        if (walkingStr != "X") {
            walking = stod(walkingStr);
            if(!graph.addBidirectionalEdge(location2, location1, walking, "walking")) exit(1);
        }

    }
    file.close();
}

/**
 * Reads input data from a file for batch processing.
 * @param inFile Path to the input file.
 * @param mode Reference to store the transportation mode.
 * @param source Reference to store the source vertex ID.
 * @param destination Reference to store the destination vertex ID.
 * @param avoidNodes Reference to store the list of nodes to avoid.
 * @param avoidSegments Reference to store the list of edges to avoid.
 * @param includeNode Reference to store the include node ID.
 * @param maxWalking Reference to store the maximum walking time allowed.
 */
void DataReader::readInputFile(const std::string& inFile, std::string& mode,
        int &source, int &destination, std::vector<int>& avoidNodes,
        std::vector<std::pair<int, int>>& avoidSegments, int& includeNode, int& maxWalking) {

    ifstream input(inFile);
    if(!input) {
        cerr << "Error opening file " << inFile << endl;
        exit(1);
    }

    string line;

    while(getline(input, line)) {
        istringstream iss(line);

        string discriminant, value;

        getline(iss, discriminant, ':');
        getline(iss, value);

        if (discriminant == "Mode") {
            mode = value;
            if (mode != "driving" && mode != "walking" && mode != "driving-walking") {
                cerr << "Invalid input." << endl;
                exit(1);
            }
        }
        else if (discriminant == "Source") {
            source = stoi(value);
        }
        else if (discriminant == "Destination") {
            destination = stoi(value);
        }
        else if (discriminant == "AvoidNodes") {
            getline(iss, value);
            if (!value.empty()) {
                std::istringstream iss(value);
                int node;
                while (iss >> node) {
                    avoidNodes.push_back(node);
                }
            }

        }
        else if (discriminant == "AvoidSegments") {
            getline(iss, value);
            if (!value.empty()) {
                std::istringstream iss(value);
                char ignore;
                int orig, dest;
                while (iss >> ignore >> orig >> ignore >> dest >> ignore) {
                    avoidSegments.emplace_back(orig, dest);
                    if (iss.peek() == ',') iss.ignore();
                }
            }
        }
        else if (discriminant == "IncludeNode") {
            getline(iss, value);
            if (!value.empty()) {
                includeNode = stoi(value);
            }
        }
        else if (discriminant == "MaxWalkTime") {
            getline(iss, value);
            if (!value.empty()) {
                maxWalking = stoi(value);
            }
        }
        else {
            cerr << "Invalid input." << endl;
            exit(1);
        }

    }
    input.close();
}




