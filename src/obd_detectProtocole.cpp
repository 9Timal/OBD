#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "../include/obd_detectProtocole.hpp"



void sendCommand(boost::asio::serial_port& serial, const std::string& cmd) {
    boost::asio::write(serial, boost::asio::buffer(cmd));
}


std::string readResponse(boost::asio::serial_port& serial) {
    std::string response;
    char c;
    boost::system::error_code ec;

    // On lit caractère par caractère jusqu’à '>' (fin de réponse ELM327)
    while (true) {
        boost::asio::read(serial, boost::asio::buffer(&c, 1), ec);
        if (ec) break;
        if (c == '>') break; // Fin de réponse ELM327
        response += c;
    }

    return response;
}

std::string detectProtocol(boost::asio::serial_port& serial) {
    sendCommand(serial, "AT DP\r");
    std::string response = readResponse(serial);

    std::cout << "🧭 Protocole détecté : " << response << std::endl;

    return response;
}
