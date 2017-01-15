#pragma once

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

#include <asio.hpp>
#include <chrono>
#include <vector>
#include <string>
#include <tuple>

using asio::ip::tcp;
typedef unsigned short ushort;

class SendData {
private:
    asio::io_service io;
    tcp::resolver resolver;
    tcp::resolver::query query;
    tcp::socket socket;
    tcp::resolver::iterator endpoint_iterator;

    std::string IP;
    ushort port;

    inline void send_string(std::string dataToSend, const char &separator = '\0') {
        if(!this->socket.is_open()) {
            asio::connect(this->socket, this->endpoint_iterator);
        }
        std::string MISTCompliant = dataToSend;
        MISTCompliant.push_back(separator);
        auto t = std::make_shared<std::thread>([=]() {
            asio::write(socket, asio::buffer(MISTCompliant.c_str(), MISTCompliant.length()));
        });
        t->detach();
    };

public:
    SendData(std::string IP, ushort port) //TODO
        : resolver(io),
          query(IP, std::to_string(port)),
          socket(io) {
       this->IP = IP;
       this->port = port;
       this->endpoint_iterator = resolver.resolve(this->query);
   };

    // EXTREME ASIO WRAPPING!!!
    DEPRECATED(void simple_send(std::string data));

    inline void send(std::string data, const char &separator = '\0') {
        send_string(data, separator);
    }

    inline std::tuple<std::string, ushort> get_raw_info() {
        return std::tuple<std::string, ushort>(this->IP, this->port);
    }
};
