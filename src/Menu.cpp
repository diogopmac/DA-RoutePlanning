//
// Created by diogo on 09/03/2025.
//
#include "../headers/Menu.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "../headers/DataReader.h"

using namespace std;

void dfsVisit(Vertex<int> *v, vector<int> & res) {
    res.push_back(v->getID());
    v->setVisited(true);
    for (auto a : v->getAdj()) {
        if (!a->getDest()->isVisited() && a->getLabel() == "drivable") {
            //
            //cout << a->getOrig()->getID() << " " << a->getDest()->getID() << endl;
            //
            dfsVisit(a->getDest(), res);
        }
    }
}

vector<int> dfs(Graph<int> *g, const std::string & source) {
    vector<int> res;
    auto v = g->findVertex(source);

    if (v == nullptr) return res;
    for (auto v : g->getVertexSet()) v->setVisited(false);

    dfsVisit(v, res);
    return res;
}

bool Menu::relax(Edge<int> *e) {
    if (e->getOrig()->getDist() + e->getWeight() >= e->getDest()->getDist()) return false;

    e->getDest()->setDist(e->getOrig()->getDist() + e->getWeight());
    e->getDest()->setPath(e);
    return true;
}

void Menu::dijkstra(Graph<int> *g, const int &start, const std::string &transportation_mode,
                    const bool alternative, const vector<int> &avoid_nodes, const vector<pair<int,int>> &avoid_edges) {

    for (auto v : g->getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
        if (!alternative) v->setVisited(false);
    }

    for (auto node : avoid_nodes) {
        g->findVertex(node)->setVisited(true);
    }

    for (auto edge : avoid_edges) {
        g->findVertex(edge.first)->findEdge(edge.second)->setAvoid(true);
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
}

std::vector<int> Menu::bestPath(Graph<int> *g, const int &start, const int &end, const std::string &transportation_mode,
                                const bool alternative=false, const vector<int> &avoid_nodes={}, const vector<pair<int,int>> &avoid_edges={}) {
    std::vector<int> res;
    dijkstra(g, start, transportation_mode, alternative, avoid_nodes, avoid_edges);
    auto v = g->findVertex(end);
    if (v == nullptr || v->getDist() == INF) return res;

    res.push_back(v->getID());
    while (v->getPath() != nullptr) {
        v = v->getPath()->getOrig();
        v->setVisited(true);
        res.push_back(v->getID());
    }

    reverse(res.begin(), res.end());
    if (res.empty() || res[0] != g->findVertex(start)->getID()) {
        std::cout << res[0] << std::endl;
        std::cout << g->findVertex(start)->getID() << std::endl;
        for (auto r : res) std::cout << r << " ";
        std::cout << "ERROR: Origin not found!" << std::endl;
    }
    return res;
}

Menu::Menu() = default;

void Menu::DefaultMenu() {
    int option;
    DataReader reader = DataReader();

    // CHANGED FOR TESTING PURPOSES
    /*
    reader.readLocations("../docs/Locations.csv", graph);
    reader.readDistances("../docs/Distances.csv", graph);
    */

    reader.readLocations("../docs/LocSample.csv", graph);
    reader.readDistances("../docs/DisSample.csv", graph);

    do {
        cout << "\nIndividual Route Planning Tool\n"
                "Desenho de Algoritmos 2025\n"
                "[1] Best Route Mode\n"
                "[2] Driving-Walking Mode\n"
                "[3] Leave application\n" << endl;
        cin >> option;
        switch (option) {
            case 1: {
                std::string mode;
                int source, destination;
                vector<int> res;
                vector<int> res2;
                vector<pair<int,int>> avoid_edges;

                cout << "Enter mode: "; cin >> mode;
                if (mode != "driving" && mode != "walking") {
                    cout << "ERROR: Wrong mode!" << endl;
                    break;
                }

                cout << "Enter Source: "; cin >> source;
                cout << "Enter Destination: "; cin >> destination;

                if (mode == "driving") {
                    res = bestPath(&graph, source, destination, "drivable");
                }
                else if (mode == "walking") {
                    res = bestPath(&graph, source, destination, "walkable");
                }

                cout << "Source:" << graph.findVertex(source)->getID() << endl;
                cout << "Destination:" << graph.findVertex(destination)->getID() << endl;
                cout << "Best Driving Route:";
                for (int i = 0; i < res.size(); i++) {
                    if (i + 1 < res.size()) {
                        avoid_edges.emplace_back(res[i], res[i+1]);
                    }
                    cout << res[i] << (i == res.size() - 1 ? "" : ",");
                }
                cout << "(" << graph.findVertex(destination)->getDist() << ")" << endl;

                if (mode == "driving") {
                    res2 = bestPath(&graph, source, destination, "drivable", true, {}, avoid_edges);
                }
                else if (mode == "walking") {
                    res2 = bestPath(&graph, source, destination, "walkable", true, {}, avoid_edges);
                }

                cout << "Source:" << graph.findVertex(source)->getID() << endl;
                cout << "Destination:" << graph.findVertex(destination)->getID() << endl;
                cout << "Best Driving Route:";
                if (graph.findVertex(destination)->getDist() == INF) {
                    cout << "none" << endl;
                }
                else {
                    for (int i = 0; i < res2.size(); i++) cout << res2[i] << (i == res2.size() - 1 ? "" : ",");
                    cout << "(" << graph.findVertex(destination)->getDist() << ")" << endl;
                }
                break;
            }

            case 2: {
                cout << "Work in progress..." << endl;
                break;
            }
            case 3:
                cout << "Leaving" << endl;
                break;
            default:
                cout << "Invalid Input!" << endl;
                break;
        }
    } while (option != 3);
}

void Menu::MenuBatchMode(const string& inFile, const string& outFile) {
    ifstream input(inFile);
    if(!input) {
        cerr << "Error opening file " << inFile << endl;
        exit(1);
    }
    ofstream output(outFile);
    if(!output) {
        cerr << "Error opening file " << outFile << endl;
        exit(1);
    }

    string line;
    while(getline(input, line)) {
        istringstream iss(line);

        string discriminant, value;

        getline(iss, discriminant, ':');
        getline(iss, value);

        if (discriminant == "Mode") {
            // TODO
        }
        else if (discriminant == "Source") {
            // TODO
        }
        else if (discriminant == "Destination") {
            // TODO
        }

    }
    input.close();
    output.close();
}






