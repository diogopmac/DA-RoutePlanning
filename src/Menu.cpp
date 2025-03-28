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

using namespace std;


bool Menu::relax(Edge<int> *e) {
    if (e->getOrig()->getDist() + e->getWeight() >= e->getDest()->getDist()) return false;

    e->getDest()->setDist(e->getOrig()->getDist() + e->getWeight());
    e->getDest()->setPath(e);
    return true;
}

void Menu::dijkstra(Graph<int> *g, const int &start, const std::string &transportation_mode,
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

std::vector<int> Menu::reconstructPath(Graph<int> *g, const int &start, const int &end, const bool reversible=true) {
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

std::vector<int> Menu::bestPath(Graph<int> *g, const int &start, const int &end, const std::string &transportation_mode,
                                const bool alternative=false, const vector<int> &avoid_nodes={}, const vector<pair<int,int>> &avoid_edges={}) {
    dijkstra(g, start, transportation_mode, alternative, avoid_nodes, avoid_edges);
    return reconstructPath(g, start, end);
}

std::pair<Path, Path> Menu::bestPathDriveWalk(Graph<int> *g, const int &start, const int &end, const int max_walking,
                                const bool alternative=false, const vector<int> &avoid_nodes={}, const vector<pair<int,int>> &avoid_edges={}, std::string &message) {
    std::map<int, Path> paths;
    std::pair<Path, Path> res;
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
        return res;
    }
    
    double lowest = INF;
    double walkTime = 0;
    dijkstra(g, end, "walking", alternative, avoid_nodes, avoid_edges);
    for (auto v : g->getVertexSet()) {
        if (!v->getParking()) continue;
        if (v->getID() == start) continue;
        if (v->getDist() > max_walking) continue;
        if (v->getID() == end) continue;

        auto p = reconstructPath(g, end, v->getID(), false);

        if (!p.empty()) {
            if (v->getDist() + paths[v->getID()].weight < lowest ||
                (v->getDist() + paths[v->getID()].weight == lowest && v->getDist() > walkTime)) {

                lowest = v->getDist() + paths[v->getID()].weight;
                walkTime = v->getDist();
                res.first = paths[v->getID()];
                res.second = {p, v->getDist()};

            }
        }
    }
    if (res.empty()) {
        message = "walking-time";
    }
    return res;
}

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

    while (mode != "driving" && mode != "walking") {
        cout << "Enter mode: "; cin >> mode;
        if (mode != "driving" && mode != "walking") {
            cout << "ERROR: Wrong mode!" << endl;
        }
    }

    while (true) {
        cout << "Enter Source: ";
        if (cin >> source) {
            if (graph.findVertex(source)) break;
            cout << "ERROR: No such vertex!" << endl;
        } else {
            cout << "ERROR: Wrong input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Enter Destination: ";
        if (cin >> destination) {
            if (graph.findVertex(destination)) break;
            cout << "ERROR: No such vertex!" << endl;
        } else {
            cout << "ERROR: Wrong input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    res = bestPath(&graph, source, destination, mode);


    cout << "Source:" << graph.findVertex(source)->getID() << endl;
    cout << "Destination:" << graph.findVertex(destination)->getID() << endl;
    cout << "BestDrivingRoute:";
    for (int i = 0; i < res.size(); i++) {
        if (i + 1 < res.size()) {
            avoid_edges.emplace_back(res[i], res[i+1]);
        }
        cout << res[i] << (i == res.size() - 1 ? "" : ",");
    }
    cout << "(" << graph.findVertex(destination)->getDist() << ")" << endl;

    res2 = bestPath(&graph, source, destination, mode, true, {}, avoid_edges);

    cout << "AlternativeDrivingRoute:";
    if (graph.findVertex(destination)->getDist() == INF) {
        cout << "none" << endl;
    }
    else {
        for (int i = 0; i < res2.size(); i++) cout << res2[i] << (i == res2.size() - 1 ? "" : ",");
        cout << "(" << graph.findVertex(destination)->getDist() << ")" << endl;
    }
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

    while (mode != "driving" && mode != "walking") {
        cout << "Enter mode: "; cin >> mode;
        if (mode != "driving" && mode != "walking") {
            cout << "ERROR: Wrong mode!" << endl;
        }
    }

    while (true) {
        cout << "Enter Source: ";
        if (cin >> source) {
            if (graph.findVertex(source)) break;
            cout << "ERROR: No such vertex!" << endl;
        } else {
            cout << "ERROR: Wrong input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Enter Destination: ";
        if (cin >> destination) {
            if (graph.findVertex(destination)) break;
            cout << "ERROR: No such vertex!" << endl;
        } else {
            cout << "ERROR: Wrong input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

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
        cout << input << endl;
        char ignore;
        int orig, dest;
        while (ss >> ignore >> orig >> ignore >> dest >> ignore) {
            avoid_edges.emplace_back(orig, dest);
            if (ss.peek() == ',') ss.ignore();
        }
    }

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

    cout << "Source:" << graph.findVertex(source)->getID() << endl;
    cout << "Destination:" << graph.findVertex(destination)->getID() << endl;
    cout << "RestrictedDrivingRoute:";

    if (includeNode == -1) {
        res = bestPath(&graph, source, destination, mode, false, avoid_nodes, avoid_edges);

        if (graph.findVertex(destination)->getDist() == INF) {
            cout << "none" << endl;
        } else {
            for (int i = 0; i < res.size(); i++) {
                if (i + 1 < res.size()) {
                    avoid_edges.emplace_back(res[i], res[i+1]);
                }
                cout << res[i] << (i == res.size() - 1 ? "" : ",");
            }
            cout << "(" << graph.findVertex(destination)->getDist() << ")" << endl;
        }
    }
    else {
        res = bestPath(&graph, source, includeNode, mode, false, avoid_nodes, avoid_edges);
        double includeDist = graph.findVertex(includeNode)->getDist();
        if (includeDist != INF) {
            res2 = bestPath(&graph, includeNode, destination, mode, true, avoid_nodes, avoid_edges);

            if (graph.findVertex(destination)->getDist() != INF) {

                for (int i = 0; i < res.size(); i++) {
                    cout << res[i] << ",";
                }
                for (int i = 1; i < res2.size(); i++) {
                    cout << res2[i] << (i == res2.size() - 1 ? "" : ",");
                }
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

    while (true) {
        cout << "Enter Source: ";
        if (cin >> source) {
            if (graph.findVertex(source)) break;
            cout << "ERROR: No such vertex!" << endl;
        } else {
            cout << "ERROR: Wrong input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Enter Destination: ";
        if (cin >> destination) {
            if (graph.findVertex(destination)) break;
            cout << "ERROR: No such vertex!" << endl;
        } else {
            cout << "ERROR: Wrong input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Enter Max Walking Time: ";
        if (cin >> maxWalking) {
            break;
        } else {
            cout << "ERROR: Wrong input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

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
        cout << input << endl;
        char ignore;
        int orig, dest;
        while (ss >> ignore >> orig >> ignore >> dest >> ignore) {
            avoid_edges.emplace_back(orig, dest);
            if (ss.peek() == ',') ss.ignore();
        }
    }

    res = bestPathDriveWalk(&graph, source, destination, maxWalking, false, avoid_nodes, avoid_edges);

    cout << "Source:" << graph.findVertex(source)->getID() << endl;
    cout << "Destination:" << graph.findVertex(destination)->getID() << endl;
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



void Menu::MenuBatchMode(const string& inFile, const string& outFile) {
    vector<int> res;
    vector<int> res2;

    ofstream out(outFile);

    string mode;
    int source, destination, includeNode;
    vector<int> avoidNodes;
    vector<pair<int,int>> avoid_edges;

    DataReader reader = DataReader();
    reader.readInputFile(inFile, mode, source, destination, avoidNodes, avoid_edges, includeNode);

    res = bestPath(&graph, source, destination, mode);

    out << "Source:" << graph.findVertex(source)->getID() << '\n';
    out << "Destination:" << graph.findVertex(destination)->getID() << '\n';
    out << "BestDrivingRoute:";
    for (int i = 0; i < res.size(); i++) {
        if (i + 1 < res.size()) {
            avoid_edges.emplace_back(res[i], res[i+1]);
        }
        out << res[i] << (i == res.size() - 1 ? "" : ",");
    }
    out << "(" << graph.findVertex(destination)->getDist() << ")" << endl;

    res2 = bestPath(&graph, source, destination, mode, true, {}, avoid_edges);

    out << "AlternativeDrivingRoute:";
    if (graph.findVertex(destination)->getDist() == INF) {
        out << "none" << endl;
    }
    else {
        for (int i = 0; i < res2.size(); i++) out << res2[i] << (i == res2.size() - 1 ? "" : ",");
        out << "(" << graph.findVertex(destination)->getDist() << ")" << endl;
    }

    out.close();

}






