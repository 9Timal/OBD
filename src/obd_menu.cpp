#include <iostream>
#include <string>
#include "../include/obd_menu.hpp"
using namespace std;
void showMenu(){
    cout << "========== MENU OBD =========\n";
    cout << "1. Lire RPM\n";
    cout << "2. Lire température moteur\n";
    cout << "3. Lire tension batterie\n";
    cout << "0. Quitter\n";
}