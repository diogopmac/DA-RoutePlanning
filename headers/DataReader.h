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
};

#endif //DATAREADER_H
