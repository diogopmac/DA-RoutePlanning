//
// Created by diogo on 09/03/2025.
//
#include "../headers/Menu.h"

#include <climits>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "../headers/DataReader.h"
#include "../headers/Dijsktra.h"

using namespace std;

Menu::Menu() = default;

void Menu::readGraph() {
    DataReader reader = DataReader();
    /*
    reader.readLocations("../docs/Locations.csv", graph);
    reader.readDistances("../docs/Distances.csv", graph);
    */

    reader.readLocations("../docs/LocSample.csv", graph);
    reader.readDistances("../docs/DisSample.csv", graph);
}

int Menu::getIntValue(const string &s, const bool &node) {
    int ret;
    while (true) {
        cout << s;
        if (cin >> ret) {
            if (node) {
                if (graph.findVertex(ret)) break;
                cout << "ERROR: No such vertex!" << endl;
            } else {
                break;
            }
        } else {
            cout << "ERROR: Wrong input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return ret;
}

std::string Menu::getTransportationMode() {
    std::string mode;
    while (mode != "driving" && mode != "walking") {
        cout << "Enter mode: "; cin >> mode;
        if (mode != "driving" && mode != "walking") {
            cout << "ERROR: Wrong mode!" << endl;
        }
    }
    return mode;
}

void Menu::getRestrictedParameters(std::vector<int> &avoid_nodes, std::vector<std::pair<int, int>> &avoid_edges) {
    cout << "Enter Avoiding Nodes: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string input;
    getline(cin, input);

    if (!input.empty()) {
        int node;
        stringstream ss(input);
        while (ss >> node) {
            avoid_nodes.push_back(node);
        }
    }

    cout << "Enter Avoiding Edges: ";
    getline(cin, input);

    if (!input.empty()) {
        stringstream ss(input);
        char ignore;
        int orig, dest;
        while (ss >> ignore >> orig >> ignore >> dest >> ignore) {
            avoid_edges.emplace_back(orig, dest);
            if (ss.peek() == ',') ss.ignore();
        }
    }
}

int Menu::getIncludeNode() {
    int includeNode;
    string input;
    while (true) {
        cout << "Enter Include Node: ";
        getline(cin, input);
        if (input.empty()) {
            includeNode = -1;
            break;
        }
        try {
            includeNode = stoi(input);
            break;
        }
        catch (invalid_argument &e) {}
    }
    return includeNode;
}

void Menu::displayInformationDriving(const int &source, const int &destination, const std::vector<int> &res, std::vector<std::pair<int, int>> &avoid_edges,
    const bool &alternative, const std::string &message) {
    if (!alternative) {
        cout << "Source:" << graph.findVertex(source)->getID() << endl;
        cout << "Destination:" << graph.findVertex(destination)->getID() << endl;
    }
    cout << message;
    if (graph.findVertex(destination)->getDist() == INF) {
        cout << "none" << endl;
    }
    else {
        for (int i = 0; i < res.size(); i++) {
            if (!alternative) {
                if (i + 1 < res.size()) avoid_edges.emplace_back(res[i], res[i+1]);
            }
            cout << res[i] << (i == res.size() - 1 ? "" : ",");
        }
        cout << "(" << graph.findVertex(destination)->getDist() << ")" << endl;
    }
}

void Menu::MainMenu() {
    int option;
    readGraph();

    do {
        cout << "\nIndividual Route Planning Tool\n"
                "Desenho de Algoritmos 2025\n"
                "[1] Best Route Mode\n"
                "[2] Restricted Route Mode\n"
                "[3] Driving-Walking Mode\n"
                "[4] Batch Mode\n"
                "[5] Leave application\n" << endl;
        cin >> option;
        switch (option) {
            case 1: {
                DefaultMenu();
                break;
            }
            case 2: {
                RestrictedMenu();
                break;
            }
            case 3: {
                MenuDrivingWalking();
                break;
            }
            case 4: {
                MenuBatchMode("../input.txt", "../output.txt");
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

void Menu::DefaultMenu() {
    string mode;
    int source, destination;
    vector<int> res;
    vector<int> res2;
    vector<pair<int,int>> avoid_edges;

    mode = getTransportationMode();
    source = getIntValue("Enter Source: ", true);
    destination = getIntValue("Enter Destination: ", true);

    res = dijkstra.bestPath(&graph, source, destination, mode);

    displayInformationDriving(source, destination, res, avoid_edges, false, "BestDrivingRoute:");

    res2 = dijkstra.bestPath(&graph, source, destination, mode, true, {}, avoid_edges);

    displayInformationDriving(source, destination, res2, avoid_edges, true, "AlternativeDrivingRoute:");
}

void Menu::RestrictedMenu() {
    string mode;
    int source, destination;
    vector<int> res;
    vector<int> res2;

    string restrictedStr;
    bool restricted;
    vector<int> avoid_nodes;
    vector<pair<int,int>> avoid_edges;
    int includeNode;

    mode = getTransportationMode();
    source = getIntValue("Enter Source: ", true);
    destination = getIntValue("Enter Destination: ", true);

    getRestrictedParameters(avoid_nodes, avoid_edges);
    includeNode = getIncludeNode();

    if (includeNode == -1) {
        res = dijkstra.bestPath(&graph, source, destination, mode, false, avoid_nodes, avoid_edges);
        displayInformationDriving(source, destination, res, avoid_edges, false, "RestrictedDrivingRoute:");
    }
    else {
        cout << "RestrictedDrivingRoute:";
        res = dijkstra.bestPath(&graph, source, includeNode, mode, false, avoid_nodes, avoid_edges);
        double includeDist = graph.findVertex(includeNode)->getDist();
        if (includeDist != INF) {
            res2 = dijkstra.bestPath(&graph, includeNode, destination, mode, true, avoid_nodes, avoid_edges);
            if (graph.findVertex(destination)->getDist() != INF) {
                for (int i = 0; i < res.size(); i++) cout << res[i] << ",";
                for (int i = 1; i < res2.size(); i++) cout << res2[i] << (i == res2.size() - 1 ? "" : ",");
                cout << "(" << includeDist + graph.findVertex(destination)->getDist() << ")" << endl;
            } else cout << "none" << endl;
        }
        else cout << "none" << endl;
    }
}

void Menu::MenuDrivingWalking() {
    int source, destination, maxWalking;
    pair<Path, Path> res;
    vector<int> avoid_nodes;
    vector<pair<int,int>> avoid_edges;
    string message;

    source = getIntValue("Enter Source: ", true);
    destination = getIntValue("Enter Destination: ", true);
    maxWalking = getIntValue("Enter Max Walking Time: ", false);
    getRestrictedParameters(avoid_nodes, avoid_edges);

    res = dijkstra.bestPathDriveWalk(&graph, source, destination, maxWalking, message, false, avoid_nodes, avoid_edges);

    cout << "Source:" << graph.findVertex(source)->getID() << endl;
    cout << "Destination:" << graph.findVertex(destination)->getID() << endl;
    if (message.empty()) {
        cout << "DrivingRoute:";
        for (int i = 0; i < res.first.path.size(); i++) {
            cout << res.first.path[i] << (i == res.first.path.size() - 1 ? "" : ",");
        } cout << "(" << res.first.weight << ")" << endl;
        cout << "ParkingNode:" << res.second.path[0] << endl;
        cout << "WalkingRoute:";
        for (int i = 0; i < res.second.path.size(); i++) {
            cout << res.second.path[i] << (i == res.second.path.size() - 1 ? "" : ",");
        } cout << "(" << res.second.weight << ")" << endl;
        cout << "TotalTime:" << res.first.weight + res.second.weight << endl;
    }
    else {
        cout << "DrivingRoute:" << endl;
        cout << "ParkingNode:" << endl;
        cout << "WalkingRoute:" << endl;
        cout << "TotalTime:" << endl;
        if (message == "walking-time") cout << "No possible route with max. walking time of " << maxWalking << " minutes." << endl;
        else if (message == "no-parking") cout << "No parking found." << endl;
    }
}



void Menu::MenuBatchMode(const string& inFile, const string& outFile) {
    vector<int> res;
    vector<int> res2;

    ofstream out(outFile);

    string mode;
    int source, destination;
    int includeNode = -1;
    vector<int> avoidNodes;
    vector<pair<int,int>> avoid_edges;

    DataReader reader = DataReader();
    reader.readInputFile(inFile, mode, source, destination, avoidNodes, avoid_edges, includeNode);

    out << "Source:" << graph.findVertex(source)->getID() << '\n';
    out << "Destination:" << graph.findVertex(destination)->getID() << '\n';

    if (includeNode == -1 && avoidNodes.empty() && avoid_edges.empty()) {
        out << "BestDrivingRoute:";

        res = dijkstra.bestPath(&graph, source, destination, mode, false, avoidNodes, avoid_edges);

        for (int i = 0; i < res.size(); i++) {
            if (i + 1 < res.size()) {
                avoid_edges.emplace_back(res[i], res[i+1]);
            }
            out << res[i] << (i == res.size() - 1 ? "" : ",");
        }
        out << "(" << graph.findVertex(destination)->getDist() << ")" << endl;

        res2 = dijkstra.bestPath(&graph, source, destination, mode, true, {}, avoid_edges);

        out << "AlternativeDrivingRoute:";
        if (graph.findVertex(destination)->getDist() == INF) {
            out << "none" << endl;
        }
        else {
            for (int i = 0; i < res2.size(); i++) out << res2[i] << (i == res2.size() - 1 ? "" : ",");
            out << "(" << graph.findVertex(destination)->getDist() << ")" << endl;
        }
    }
    else {
        out << "RestrictedDrivingRoute:";

        if (includeNode == -1) {
            res = dijkstra.bestPath(&graph, source, destination, mode, false, avoidNodes, avoid_edges);

            if (graph.findVertex(destination)->getDist() == INF) {
                out << "none" << '\n';
            } else {
                for (int i = 0; i < res.size(); i++) {
                    if (i + 1 < res.size()) {
                        avoid_edges.emplace_back(res[i], res[i+1]);
                    }
                    out << res[i] << (i == res.size() - 1 ? "" : ",");
                }
                out << "(" << graph.findVertex(destination)->getDist() << ")" << '\n';
            }
        }
        else {
            res = dijkstra.bestPath(&graph, source, includeNode, mode, false, avoidNodes, avoid_edges);
            double includeDist = graph.findVertex(includeNode)->getDist();
            if (includeDist != INF) {
                res2 = dijkstra.bestPath(&graph, includeNode, destination, mode, true, avoidNodes, avoid_edges);

                if (graph.findVertex(destination)->getDist() != INF) {

                    for (int i = 0; i < res.size(); i++) {
                        out << res[i] << ",";
                    }
                    for (int i = 1; i < res2.size(); i++) {
                        out << res2[i] << (i == res2.size() - 1 ? "" : ",");
                    }
                    out << "(" << includeDist + graph.findVertex(destination)->getDist() << ")" << '\n';

                } else out << "none" << '\n';
            }
            else out << "none" << '\n';
        }
    }


    out.close();

}






