//
// Created by diogo on 11/03/2025.
//

#ifndef DATAREADER_H
#define DATAREADER_H
#include <string>
#include "Graph.h"

class DataReader {
    public:
    DataReader();
    ~DataReader();
    void readLocations(const std::string fileName);
    void readDistances(const std::string fileName);
};

#endif //DATAREADER_H
