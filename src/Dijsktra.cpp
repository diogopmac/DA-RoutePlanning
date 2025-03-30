//
// Created by diogo on 29/03/2025.
//

#include "../headers/Dijsktra.h"

#include <climits>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;

/**
 * Relaxes an edge in the graph updating the destination vertex's distance and path is shorter path is found.
 * @param e Pointer to the edge to be relaxed
 * @return True if edge was relaxed, false otherwise
 * @note Time Complexity:
 * - O(1) in all cases.
 */
bool Dijkstra::relax(Edge<int> *e) {
    if (e->getOrig()->getDist() + e->getWeight() >= e->getDest()->getDist()) return false;

    e->getDest()->setDist(e->getOrig()->getDist() + e->getWeight());
    e->getDest()->setPath(e);
    return true;
}

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
void Dijkstra::dijkstra(Graph<int> *g, const int &start, const std::string &transportation_mode,
                    const bool alternative, const vector<int> &avoid_nodes, const vector<pair<int,int>> &avoid_edges) {
    vector<Edge<int> *> edges_to_revert;

    for (auto v : g->getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
        if (!alternative) v->setVisited(false);
    }

    for (auto node : avoid_nodes) {
        g->findVertex(node)->setVisited(true);
    }

    for (auto edge : avoid_edges) {
        auto e = g->findVertex(edge.first)->findEdge(edge.second);
        e->setAvoid(true);
        edges_to_revert.push_back(e);
    }

    auto s = g->findVertex(start);
    s->setDist(0);

    MutablePriorityQueue<Vertex<int>> q;
    q.insert(s);
    while (!q.empty()) {
        auto v = q.extractMin();
        for (auto e : v->getAdj()) {
            if (e->getLabel() == transportation_mode && !e->getDest()->isVisited() && !e->shouldAvoid()) {
                auto dist_old = e->getDest()->getDist();
                if (relax(e)) {
                    if (dist_old == INF) {
                        q.insert(e->getDest());
                    } else {
                        q.decreaseKey(e->getDest());
                    }
                }
            }
        }
    }
    for (auto e : edges_to_revert) {
        e->setAvoid(false);
    }
}

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
std::vector<int> Dijkstra::reconstructPath(Graph<int> *g, const int &start, const int &end, const bool reversible=true) {
    std::vector<int> res;

    auto v = g->findVertex(end);
    if (v == nullptr || v->getDist() == INF) return res;

    res.push_back(v->getID());
    while (v->getPath() != nullptr) {
        v = v->getPath()->getOrig();
        v->setVisited(true);
        res.push_back(v->getID());
    }

    if (reversible) reverse(res.begin(), res.end());
    if (res.empty() || (reversible ? res[0] : res.back()) != g->findVertex(start)->getID()) {
        std::cout << res[0] << std::endl;
        std::cout << g->findVertex(start)->getID() << std::endl;
        for (auto r : res) std::cout << r << " ";
        std::cout << "ERROR: Origin not found!" << std::endl;
    }
    return res;
}

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
std::vector<int> Dijkstra::bestPath(Graph<int> *g, const int &start, const int &end, const std::string &transportation_mode,
                                const bool alternative, const vector<int> &avoid_nodes, const vector<pair<int,int>> &avoid_edges) {
    dijkstra(g, start, transportation_mode, alternative, avoid_nodes, avoid_edges);
    return reconstructPath(g, start, end);
}

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
std::pair<std::pair<Path, Path>, std::pair<Path, Path>> Dijkstra::bestPathDriveWalk(Graph<int> *g, const int &start, const int &end, const int max_walking, std::string &message,
                                const bool alternative=false, const vector<int> &avoid_nodes={}, const vector<pair<int,int>> &avoid_edges={}) {
    std::map<int, Path> paths;
    std::pair<Path, Path> res, res2;
    dijkstra(g, start, "driving", alternative, avoid_nodes, avoid_edges);
    for (auto v : g->getVertexSet()) {
        if (!v->getParking()) continue;
        if (v->getID() == start) continue;
        if (v->getID() == end) continue;

        auto p = reconstructPath(g, start, v->getID());

        if (!p.empty()) {
            paths[v->getID()] = {p, v->getDist()};
        }
    }
    if (paths.empty()) {
        message = "no-parking";
        if (max_walking == INF) {
            message = "no-parking-alternative";
            return {res, res2};
        }
        return bestPathDriveWalk(g, start, end, INF, message);
    }

    double lowest = INF;
    double lowestAlt = INF;
    double walkTime = 0;
    bool valid_walkTime = false;
    dijkstra(g, end, "walking", alternative, avoid_nodes, avoid_edges);
    for (auto v : g->getVertexSet()) {
        if (!v->getParking()) continue;
        if (v->getID() == start) continue;
        if (v->getID() == end) continue;
        if (v->getDist() > max_walking) continue;
        valid_walkTime = true;

        auto p = reconstructPath(g, end, v->getID(), false);

        if (!p.empty()) {
            double pathWeight = v->getDist() + paths[v->getID()].weight;
            if ((pathWeight < lowest) || (pathWeight == lowest && v->getDist() > walkTime)) {

                lowestAlt = lowest;
                lowest = v->getDist() + paths[v->getID()].weight;
                walkTime = v->getDist();
                res2 = res;
                res.first = paths[v->getID()];
                res.second = {p, v->getDist()};
            }
            else if (pathWeight < lowestAlt) {
                lowestAlt = v->getDist() + paths[v->getID()].weight;
                res2.first = paths[v->getID()];
                res2.second = {p, v->getDist()};
            }
        }
    }
    if (!valid_walkTime) {
        message = "walking-time";
        if (max_walking == INF) {
            message = "walking-time-alternative";
            return {res, res2};
        }
        return bestPathDriveWalk(g, start, end, INF, message);
    }
    return {res, res2};
}

