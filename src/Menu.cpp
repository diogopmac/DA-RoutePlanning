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






