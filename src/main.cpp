#include <iostream>
#include <string>
#include "../headers/graph.h"
#include "../headers/Menu.h"

int main(int argc , char *argv[]) {
    Menu menu;
    if (argc == 3) {
        const std::string inFile = argv[1];
        const std::string outFile = argv[2];
        menu.MenuBatchMode("../" + inFile, "../" + outFile);
    }
    else {
        menu.DefaultMenu();
    }
    return 0;
}
