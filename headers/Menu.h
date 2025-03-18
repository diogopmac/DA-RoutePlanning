//
// Created by diogo on 09/03/2025.
//

#ifndef MENU_H
#define MENU_H
#include "graph.h"
#include "DataReader.h"
#include <string>

class Menu {
private:
    Graph<int> graph;
    DataReader reader;
public:
    Menu();
    void DefaultMenu();
    void MenuBatchMode(const std::string& inFile, const std::string& outFile);
};



#endif //MENU_H
