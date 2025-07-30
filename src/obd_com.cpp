#include <boost/asio.hpp>
#include "../include/obd_com.hpp"
#include <iostream>



bool setupSerialPort(boost::asio::serial_port& serial, const std::string& portName, boost::asio::io_context& io) {
    (void)io; // évite le warning
    boost::system::error_code ec;
    serial.open(portName, ec);
    if (ec) {
        std::cerr << "❌ Erreur ouverture port " << portName << " : " << ec.message() << std::endl;
        return false;
    }

    serial.set_option(boost::asio::serial_port_base::baud_rate(38400));
    serial.set_option(boost::asio::serial_port_base::character_size(8));
    serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

    return true;
}
