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

bool Dijkstra::relax(Edge<int> *e) {
    if (e->getOrig()->getDist() + e->getWeight() >= e->getDest()->getDist()) return false;

    e->getDest()->setDist(e->getOrig()->getDist() + e->getWeight());
    e->getDest()->setPath(e);
    return true;
}

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

std::vector<int> Dijkstra::bestPath(Graph<int> *g, const int &start, const int &end, const std::string &transportation_mode,
                                const bool alternative, const vector<int> &avoid_nodes, const vector<pair<int,int>> &avoid_edges) {
    dijkstra(g, start, transportation_mode, alternative, avoid_nodes, avoid_edges);
    return reconstructPath(g, start, end);
}

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
            if (v->getDist() + paths[v->getID()].weight < lowest ||
                (v->getDist() + paths[v->getID()].weight == lowest && v->getDist() > walkTime)) {

                lowest = v->getDist() + paths[v->getID()].weight;
                walkTime = v->getDist();
                res2 = res;
                res.first = paths[v->getID()];
                res.second = {p, v->getDist()};

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

