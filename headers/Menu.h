//
// Created by diogo on 09/03/2025.
//

#ifndef MENU_H
#define MENU_H
#include "graph.h"
#include "DataReader.h"
#include "Dijsktra.h"
#include <string>

class Menu {
private:
    Graph<int> graph;
    DataReader reader;
    Dijkstra dijkstra;
public:
    /**
     * Constructor for the Menu class.
     */
    Menu();
    /**
     * Displays the main menu and processes user input choices.
     */
    void MainMenu();
    /**
     * Displays the default route planning menu and processes user input.
     */
    void DefaultMenu();
    /**
     * Displays the restricted route planning menu and processes user input.
     */
    void RestrictedMenu();
    /**
     * Processes batch mode operations from input file and writes output to file.
     * @param inFile Path to input file.
     * @param outFile Path to output file.
     */
    void MenuBatchMode(const std::string& inFile, const std::string& outFile);
    /**
     * Displays the driving-walking mode menu and processes user input.
     */
    void MenuDrivingWalking();

    /**
     * Reads the graph data from files.
     */
    void readGraph();

    /**
     * Gets an integer value from user input.
     * @param s The message prompt for the user.
     * @param node Boolean value that indicates whether to check for valid node.
     * @return The integer input validated.
     */
    int getIntValue(const std::string &s, const bool &node);
    /**
     * Gets the transportation mode from user input.
     * @return A string representing the selected transportation mode.
     */
    std::string getTransportationMode();
    /**
     * Gets restricted route parameters from user input.
     * @param avoid_nodes Reference to a vector of avoiding nodes.
     * @param avoid_edges Reference to a vector of avoiding edges.
     */
    void getRestrictedParameters(std::vector<int> &avoid_nodes, std::vector<std::pair<int, int>> &avoid_edges);
    /**
     * Gets a node that must be included in the path.
     * @return The node to be included or -1 if no node was specified.
     */
    int getIncludeNode();
    /**
     * Displays information about the driving route.
     * @param source Source node.
     * @param destination Destination node.
     * @param res Route result vector.
     * @param avoid_edges Edges to be avoided.
     * @param alternative Boolean indication if it is an alternative route or not
     * @param message Message to be displayed
     */
    void displayInformationDriving(const int &source, const int &destination, const std::vector<int> &res, std::vector<std::pair<int, int>> &avoid_edges, const bool &alternative, const std::string &message);
};



#endif //MENU_H
