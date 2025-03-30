#include <iostream>
#include <string>
#include "../headers/graph.h"
#include "../headers/Menu.h"

/**
 * Entry point of the application.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status of the program.
 */
int main(int argc , char *argv[]) {
    Menu menu;
    if (argc == 3) {
        menu.readGraph();
        const std::string inFile = argv[1];
        const std::string outFile = argv[2];
        menu.MenuBatchMode("../" + inFile, "../" + outFile);
    }
    else {
        menu.MainMenu();
    }
    return 0;
}
