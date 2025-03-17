#include <iostream>
#include <string>
#include "../headers/Graph.h"
#include "../headers/Menu.h"

int main(int argc , char *argv[]) {
    Graph<int> *graph;
    Menu menu;
    if (argc == 3) {
        const std::string inFile = argv[1];
        const std::string outFile = argv[2];
        menu.MenuBatchMode("../input.txt", "../output.txt");
    }
    else {
        menu.DefaultMenu();
    }
    return 0;
}
