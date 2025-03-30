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
        /**
         * Reconstructs the shortest path from the start node to the end node.
         * @param g Pointer to the graph.
         * @param start Starting node (source).
         * @param end Ending node (destination).
         * @param reversible Boolean value that indicates if path should be reversible.
         * @return Vector containing the sequence of nodes in the shortest path.
         * @note Timme Complexity:
         * - Best-case: O(1) if the destination node is unreachable.
         * - Average-case: O(V) when reconstructing a typical path.
         * - Worst-case: O(V) if the graph is a single long path.
         */
        std::vector<int> reconstructPath(Graph<int> *g, const int &start, const int &end, const bool reversible);
        /**
        * Finds the best path between two nodes in the graph.
        * @param g Pointer to the graph.
        * @param start Starting node
        * @param end Ending node.
        * @param transportation_mode Mode of transportation.
        * @param alternative Boolean flag to indicate if an alternative route is needed.
        * @param avoid_nodes List of nodes to avoid.
        * @param avoid_edges List of edges to avoid.
        * @return Vector containing the best path between the nodes.
        * @note Time Complexity:
        * - Best-case: O(V log V) if the graph has no edges.
        * - Average-case: O((V + E) log V) in a typical graph.
        * - Worst-case: O((V + E) log V) in a fully connected graph.
        */
        std::vector<int> bestPath(Graph<int> *g, const int &start, const int &end, const std::string &transportation_mode, bool alternative=false, const std::vector<int> &avoid_nodes={}, const std::vector<std::pair<int, int>> &avoid_edges={});
        /**
         * Finds the best combined driving and walking path between two nodes.
         * @param g Pointer to the graph.
         * @param start Starting node.
         * @param end Ending node.
         * @param max_walking The maximum walking time allowed.
         * @param message Reference to a message string describing the status.
         * @param alternative Boolean flag to indicate if an alternative route is needed.
         * @param avoid_nodes List of nodes to avoid.
         * @param avoid_edges List of edges to avoid.
         * @return Pair of paths representing the driving and walking routes.
         * @note Time Complexity:
         * - Best-case: O((V + E) log V) if a parking spot is found quickly.
         * - Average-case: O((V + E) log V + P V), where P is the number of parking nodes (it is significantly smaller than V).
         * - Worst-case: O((V + E) log V + V^2) if every node is a parking node.
         */
        std::pair<std::pair<Path, Path>, std::pair<Path, Path>> bestPathDriveWalk(Graph<int> *g, const int &start, const int &end, int max_walking, std::string &message, bool alternative, const std::vector<int> &avoid_nodes, const std::vector<std::pair<int, int>> &avoid_edges);
        /**
        * Implements Dijkstra's algorithm to find the shortest path in the graph.
        * @param g Pointer to the graph.
        * @param start Starting node.
         * @param transportation_mode Mode of transportation.
         * @param alternative Boolean value indicating if an alternative route is needed or not.
         * @param avoid_nodes List of nodes to avoid.
         * @param avoid_edges List of edges to avoid.
         * @note Time Complexity:
         * - Best-case: O(V log V) if there are no edges.
         * - Average-case: O((V + E) log V) in a typical graph.
         * - Worst-case: O((V + E) log V) in a fully connected graph.
         */
        void dijkstra(Graph<int> *g, const int &start, const std::string &transportation_mode, bool alternative, const std::vector<int> &avoid_nodes, const std::vector<std::pair<int, int>> &avoid_edges);
        /**
        * Relaxes an edge in the graph updating the destination vertex's distance and path is shorter path is found.
        * @param e Pointer to the edge to be relaxed
        * @return True if edge was relaxed, false otherwise
        * @note Time Complexity:
        * - O(1) in all cases.
        */
        bool relax(Edge<int> *e);
};

#endif //DIJSKTRA_H
