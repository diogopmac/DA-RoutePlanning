//
// Created by diogo on 09/03/2025.
//
#include "../headers/Menu.h"
#include <iostream>

using namespace std;

void Menu() {
    int option;

    do {
        cout << "Route Planner" << endl;
        cout << "1 - Load" << endl;
        cout << "2 - Show map" << endl;
        cout << "3 - Leave" << endl;
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