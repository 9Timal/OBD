#include <boost/asio.hpp>
#include <iostream>
#include "../include/obd_device.hpp"

int Device() {
    boost::asio::io_context io;
    boost::asio::serial_port port(io);

    std::cout << "Tentative d'ouverture du port série..." << std::endl;

    port.open("COM3"); // adapte selon ton port
    port.set_option(boost::asio::serial_port_base::baud_rate(9600));

    std::string cmd = "ATI\r";
    boost::asio::write(port, boost::asio::buffer(cmd));

    char reply[128];
    size_t n = port.read_some(boost::asio::buffer(reply));
    std::cout << "Réponse : " << std::string(reply, n) << std::endl;

    std::cout << "Lecture terminée." << std::endl;


    return 0;
}