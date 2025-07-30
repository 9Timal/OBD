// obd_com.hpp
#ifndef OBD_COM_HPP
#define OBD_COM_HPP
#include <boost/asio.hpp>
#include <string>


bool setupSerialPort(boost::asio::serial_port& serial, const std::string& portName, boost::asio::io_context& io);

#endif