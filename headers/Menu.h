//
// Created by diogo on 09/03/2025.
//

#ifndef MENU_H
#define MENU_H
#include "graph.h"
#include "DataReader.h"
#include "Dijsktra.h"
#include <string>

class Menu {
private:
    Graph<int> graph;
    DataReader reader;
    Dijkstra dijkstra;
public:
    Menu();
    void MainMenu();
    void DefaultMenu();
    void RestrictedMenu();
    void MenuBatchMode(const std::string& inFile, const std::string& outFile);
    void MenuDrivingWalking();

    void readGraph();

    int getIntValue(const std::string &s, const bool &node);
    std::string getTransportationMode();
    void getRestrictedParameters(std::vector<int> &avoid_nodes, std::vector<std::pair<int, int>> &avoid_edges);
    int getIncludeNode();
    void displayInformationDriving(const int &source, const int &destination, const std::vector<int> &res, std::vector<std::pair<int, int>> &avoid_edges, const bool &alternative, const std::string &message);
};



#endif //MENU_H
