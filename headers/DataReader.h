//
// Created by diogo on 11/03/2025.
//

#ifndef DATAREADER_H
#define DATAREADER_H
#include <string>
#include "graph.h"

class DataReader {
    public:
    DataReader();
    ~DataReader();
    void readLocations(const std::string& fileName, Graph<int>& graph);
    void readDistances(const std::string& fileName, Graph<int>& graph);
    void readInputFile(const std::string& inFile, std::string& mode,
        int &source, int &destination, std::vector<int>& avoidNodes,
        std::vector<std::pair<int, int>>& avoidSegments, int& includeNode, int& maxWalking);
};

#endif //DATAREADER_H
