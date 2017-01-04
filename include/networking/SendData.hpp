#pragma once
//#include "MIST_Internal.hpp"
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

    inline size_t send_string(std::string dataToSend, const char &separator = '\0') {
        if(!this->socket.is_open()) {
            asio::connect(this->socket, this->endpoint_iterator);
        }
        std::string MISTCompliant = dataToSend;
        MISTCompliant.push_back(separator);
        return asio::write(socket, asio::buffer(MISTCompliant.c_str(), MISTCompliant.length()));
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
    ~SendData() { };

    //EXTREME ASIO WRAPPING!!!
    inline size_t Send(std::string data, const char &separator) {
        return send_string(data, separator);
    };

    inline void simple_send(std::string data) {
        send_string(data);
    }

    inline std::tuple<std::string, ushort> get_raw_info() {
        return std::tuple<std::string, ushort>(this->IP, this->port);
    }
};
