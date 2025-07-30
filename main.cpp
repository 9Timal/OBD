#include "include/obd_detectPort.hpp"
#include "include/obd_detectProtocole.hpp"
#include "include/obd_com.hpp"
#include "include/obd_menu.hpp"
#include "include/obd_device.hpp"
#include "include/obd_keyboard.hpp"
#include <iostream>
#include <string>
#include <string>
#include <windows.h>



using namespace std;


int main() {
    std::string portDetected = detectOBDPort();

    if (!portDetected.empty()) {
        std::cout << "Port détecté : " << portDetected << std::endl;
        boost::asio::io_context io;
        boost::asio::serial_port serial(io);

        std::cout << "Configuration de la ommunication ..."  << std::endl;
        bool config = setupSerialPort(serial, portDetected, io);
        
        if (config){
            std::string protocole = detectProtocol(serial);
            std::cout << "Portocole utilisé : " << protocole << std::endl;
            
            showMenu();
    
            char choice = waitForKey(); 
            cout << "Choix : " << choice << endl;
    
            switch (choice) {
                case '1':
                    std::cout << "🛈 Détection des informations du véhicule...\n";
                    
                    break;
                case '2':
                    std::cout << "🔍 Scan du véhicule en cours...\n";
                    // TODO: appeler ta fonction ici
                    break;
                case '3':
                    std::cout << "☎️ Contact entreprise : contact@obdtool.com\n";
                    break;
                default:
                    std::cout << "Choix invalide.\n";
                    break;
            }
    
        } else {
            std::cout << "Aucun port OBD détecté. Vérifiez le branchement de votre port OBD !" << std::endl;
        }

    }

    std::cout << "\nAppuyez sur [ESC] pour quitter...\n";
    char key = waitForKey();

    if (isEscape(key)) {
        std::cout << "Programme terminé.\n";
    }
}