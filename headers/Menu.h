//
// Created by diogo on 09/03/2025.
//

#ifndef MENU_H
#define MENU_H
#include "graph.h"
#include "DataReader.h"
#include <string>

struct Path {
    std::vector<int> path;
    double weight;
};

class Menu {
private:
    Graph<int> graph;
    DataReader reader;
public:
    Menu();
    void MainMenu();
    void DefaultMenu();
    void RestrictedMenu();
    void MenuBatchMode(const std::string& inFile, const std::string& outFile);
    void MenuDrivingWalking();
    void readGraph();

    std::vector<int> reconstructPath(Graph<int> *g, const int &start, const int &end, const bool reversible);
    std::vector<int> bestPath(Graph<int> *g, const int &start, const int &end, const std::string &transportation_mode, bool alternative, const std::vector<int> &avoid_nodes, const std::vector<std::pair<int, int>> &avoid_edges);
    std::pair<Path, Path> bestPathDriveWalk(Graph<int> *g, const int &start, const int &end, int max_walking, std::string &message, bool alternative, const std::vector<int> &avoid_nodes, const std::vector<std::pair<int, int>> &avoid_edges);
    void dijkstra(Graph<int> *g, const int &start, const std::string &transportation_mode, bool alternative, const std::vector<int> &avoid_nodes, const std::vector<std::pair<int, int>> &avoid_edges);
    bool relax(Edge<int> *e);
};



#endif //MENU_H
