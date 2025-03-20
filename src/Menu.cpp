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

template <class T>
bool relax(Edge<T> *e) {
    if (e->getOrig()->getDist() + e->getWeight() >= e->getDest()->getDist()) return false;

    e->getDest()->setDist(e->getOrig()->getDist() + e->getWeight());
    e->getDest()->setPath(e);
    return true;
}

template <class T>
void dijkstra(Graph<T> *g, const std::string &start) {
    for (auto v : g->getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
    }
    auto s = g->findVertex(start);
    s->setDist(0);

    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while (!q.empty()) {
        auto v = q.extractMin();
        for (auto e : v->getAdj()) {
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

template <class T>
std::vector<T> bestPath(Graph<T> *g, const std::string &start, const std::string &end) {
    std::vector<T> res;
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
                "[2] Show Map\n"
                "[3] Leave application\n" << endl;
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
                vector<int> res = dfs(&graph, "TR2349");
                vector<int> res2 = bestPath(&graph, "CALE", "JP6553");

                /*
                for (auto v : res) cout << v << " ";
                */

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






