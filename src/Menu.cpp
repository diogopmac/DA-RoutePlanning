//
// Created by diogo on 09/03/2025.
//
#include "../headers/Menu.h"
#include <iostream>

using namespace std;

void Menu() {
    int option;

    do {
        cout << "\nIndividual Route Planning Tool\n"
                "Desenho de Algoritmos 2025\n"
                "[1] Load Map\n"
                "[2] Show Map\n"
                "[3] Leave application\n" << endl;
        cin >> option;
        switch (option) {
            case 1:
                cout << "WIP" << endl;
                break;
            case 2:
                cout << "WIP" << endl;
                break;
            case 3:
                cout << "Leaving" << endl;
                break;
            default:
                cout << "Invalid Input!" << endl;
        }
    } while (option != 3);
}