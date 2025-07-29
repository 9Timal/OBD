#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "../include/obd_detectPort.hpp"
#include <future>
#include <windows.h>
#include <initguid.h>
#include <devguid.h>
#include <setupapi.h>
#include <tchar.h>
#include <vector>


struct SerialPortInfo {
    std::string portName;
    std::string description;
};

std::vector<SerialPortInfo> getSerialPorts() {
    std::vector<SerialPortInfo> result;

    HDEVINFO hDevInfo = SetupDiGetClassDevs(
        &GUID_DEVCLASS_PORTS,
        nullptr,
        nullptr,
        DIGCF_PRESENT
    );

    if (hDevInfo == INVALID_HANDLE_VALUE)
        return result;

    SP_DEVINFO_DATA devInfo = {};
    devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

    char portName[256];
    char friendlyName[256];

    for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfo); ++i) {
        HKEY hKey = SetupDiOpenDevRegKey(hDevInfo, &devInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
        if (hKey == INVALID_HANDLE_VALUE) continue;

        DWORD type = 0, size = sizeof(portName);
        if (RegQueryValueExA(hKey, "PortName", nullptr, &type, (LPBYTE)portName, &size) == ERROR_SUCCESS) {
            if (type == REG_SZ) {
                SetupDiGetDeviceRegistryPropertyA(
                    hDevInfo, &devInfo, SPDRP_FRIENDLYNAME, nullptr,
                    (PBYTE)friendlyName, sizeof(friendlyName), nullptr
                );

                result.push_back({
                    portName,
                    friendlyName
                });
            }
        }

        RegCloseKey(hKey);
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return result;
}

std::string detectOBDPort() {
    using namespace boost::asio;

    auto ports = getSerialPorts();

    for (const auto& info : ports) {
        const std::string& portName = info.portName;
        const std::string& description = info.description;

        // Ignorer les ports Bluetooth ou non désirés
        if (description.find("Bluetooth") != std::string::npos) {
            std::cout << "⏭️ " << portName << " ignoré (Bluetooth détecté : " << description << ")" << std::endl;
            continue;
        }

        std::cout << "\n🔍 Test du port : " << portName << " (" << description << ") ... ";

        try {
            io_context io;
            serial_port serial(io, "\\\\.\\" + portName); // important pour COM10+ sur Windows

            serial.set_option(serial_port_base::baud_rate(9600));
            serial.set_option(serial_port_base::character_size(8));
            serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
            serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
            serial.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

            std::cout << "✅ Ouvert. Envoi de 'ATI\\r'..." << std::endl;

            std::string cmd = "ATI\r";
            write(serial, buffer(cmd));

            std::promise<std::string> responsePromise;
            auto responseFuture = responsePromise.get_future();

            std::thread readerThread([&serial, &responsePromise]() {
                try {
                    char response[128] = {0};
                    std::size_t len = serial.read_some(buffer(response));
                    responsePromise.set_value(std::string(response, len));
                } catch (...) {
                    responsePromise.set_value("");
                }
            });

            if (responseFuture.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
                std::string reply = responseFuture.get();
                if (!reply.empty()) {
                    std::cout << "📥 Réponse : " << reply << std::endl;
                    if (reply.find("ELM") != std::string::npos) {
                        std::cout << "✅ Port OBD détecté !" << std::endl;
                        readerThread.join();
                        return portName;
                    } else {
                        std::cout << "❌ Pas de signature ELM." << std::endl;
                    }
                } else {
                    std::cout << "❌ Lecture échouée." << std::endl;
                }
            } else {
                std::cout << "⏱️ Timeout — aucune réponse." << std::endl;
                readerThread.detach();
            }

        } catch (const std::exception& ex) {
            std::cout << "❌ Erreur : " << ex.what() << std::endl;
        }
    }

    std::cout << "\n🚫 Aucun port OBD détecté." << std::endl;
    return "";
}

