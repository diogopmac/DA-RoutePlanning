//
// Created by diogo on 09/03/2025.
//

#ifndef MENU_H
#define MENU_H
#include "graph.h"
#include "DataReader.h"
#include <string>

class Menu {
private:
    Graph<int> graph;
    DataReader reader;
public:
    Menu();
    void DefaultMenu();
    void MenuBatchMode(const std::string& inFile, const std::string& outFile);

    std::vector<int> bestPath(Graph<int> *g, const int &start, const int &end, const std::string& label);
    void dijkstra(Graph<int> *g, const int &start, const std::string &label);
    bool relax(Edge<int> *e);
};



#endif //MENU_H
