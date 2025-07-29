#include <iostream>
#include <string>
#include "include/obd_menu.hpp"
#include "include/obd_device.hpp"
#include "include/obd_detectPort.hpp"
using namespace std;
#include <string>
//import pour lire les touches utilisateurs
#include "include/obd_keyboard.hpp"

int main() {
    std::string portDetected = detectOBDPort();

    if (!portDetected.empty()) {
        std::cout << "Port détecté : " << portDetected << std::endl;
    } else {
        std::cout << "Aucun port OBD détecté." << std::endl;
    }

    std::cout << "\nAppuyez sur [ESC] pour quitter...\n";
    char key = waitForKey();

    if (isEscape(key)) {
        std::cout << "Programme terminé.\n";
    }
}
