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

void Menu::dijkstra(Graph<int> *g, const std::string &start, const std::string &label) {
    for (auto v : g->getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
    }
    auto s = g->findVertex(start);
    s->setDist(0);

    MutablePriorityQueue<Vertex<int>> q;
    q.insert(s);
    while (!q.empty()) {
        auto v = q.extractMin();
        for (auto e : v->getAdj()) {
            if (e->getLabel() == label) {
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

std::vector<int> Menu::bestPath(Graph<int> *g, const std::string &start, const std::string &end, const std::string& label) {
    std::vector<int> res;
    dijkstra(g, start, label);
    auto v = g->findVertex(end);
    if (v == nullptr || v->getDist() == INF) return res;

    res.push_back(v->getID());
    while (v->getPath() != nullptr) {
        v = v->getPath()->getOrig();
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

    do {
        cout << "\nIndividual Route Planning Tool\n"
                "Desenho de Algoritmos 2025\n"
                "[1] Load Map\n"
                "[2] Driving Mode\n"
                "[3] Walking Mode\n"
                "[4] Driving-Walking Mode\n"
                "[5] Leave application\n" << endl;
        cin >> option;
        switch (option) {
            case 1:
                // CHANGED FOR TESTING PURPOSES
                /*
                reader.readLocations("../docs/Locations.csv", graph);
                reader.readDistances("../docs/Distances.csv", graph);
                */

                reader.readLocations("../docs/LocSample.csv", graph);
                reader.readDistances("../docs/DisSample.csv", graph);

                break;
            case 2: {
                string source, destination;
                cout << "Enter Source: "; cin >> source;
                cout << "Enter Destination: "; cin >> destination;
                vector<int> res = bestPath(&graph, source, destination, "drivable");

                cout << "Source : " << graph.findVertex(source)->getID() << endl;
                cout << "Destination : " << graph.findVertex(destination)->getID() << endl;
                cout << "Best Driving Route: ";
                for (auto v : res) cout << v << " ";
                cout << "(" << graph.findVertex(destination)->getDist() << ")" << endl;

                break;
            }

            case 3: {
                string source, destination;
                cout << "Enter Source: "; cin >> source;
                cout << "Enter Destination: "; cin >> destination;
                vector<int> res = bestPath(&graph, source, destination, "walkable");

                cout << "Source : " << graph.findVertex(source)->getID() << endl;
                cout << "Destination : " << graph.findVertex(destination)->getID() << endl;
                cout << "Best Driving Route: ";
                for (auto v : res) cout << v << " ";
                cout << "(" << graph.findVertex(destination)->getDist() << ")" << endl;

                break;
            }
            case 5:
                cout << "Leaving" << endl;
                break;
            default:
                cout << "Invalid Input!" << endl;
                break;
        }
    } while (option != 5);
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






