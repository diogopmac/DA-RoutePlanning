//
// Created by diogo on 29/03/2025.
//

#ifndef DIJSKTRA_H
#define DIJSKTRA_H

#include "graph.h"

struct Path {
    std::vector<int> path;
    double weight;
};

class Dijkstra {
    public:
        std::vector<int> reconstructPath(Graph<int> *g, const int &start, const int &end, const bool reversible);
        std::vector<int> bestPath(Graph<int> *g, const int &start, const int &end, const std::string &transportation_mode, bool alternative=false, const std::vector<int> &avoid_nodes={}, const std::vector<std::pair<int, int>> &avoid_edges={});
        std::pair<Path, Path> bestPathDriveWalk(Graph<int> *g, const int &start, const int &end, int max_walking, std::string &message, bool alternative, const std::vector<int> &avoid_nodes, const std::vector<std::pair<int, int>> &avoid_edges);
        void dijkstra(Graph<int> *g, const int &start, const std::string &transportation_mode, bool alternative, const std::vector<int> &avoid_nodes, const std::vector<std::pair<int, int>> &avoid_edges);
        bool relax(Edge<int> *e);
};

#endif //DIJSKTRA_H
