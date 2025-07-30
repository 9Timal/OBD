// obd_detectProtocole.hpp
#ifndef OBD_DETECTPROTOCOLE_HPP
#define OBD_DETECTPROTOCOLE_HPP
#include <boost/asio.hpp>
#include <string>

std::string detectProtocol(boost::asio::serial_port& serial);
void sendCommand(boost::asio::serial_port& serial, const std::string& cmd);
std::string readResponse(boost::asio::serial_port& serial);

#endif