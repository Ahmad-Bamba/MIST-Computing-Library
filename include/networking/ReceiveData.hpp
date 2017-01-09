#pragma once

#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <asio.hpp>

using asio::ip::tcp;

typedef unsigned short ushort;

class ReceiveData {
private:
    asio::io_service service;
    tcp::acceptor acceptor;
    tcp::socket socket;
public:
    ReceiveData(ushort port = 8008) : acceptor(service, tcp::endpoint(tcp::v4(), port)),
                               socket(service) { }

    //Can return any amount on the socket stream
    template<size_t N>
    inline std::string receive() {
        std::string message;
        try {
            if (!this->socket.is_open()) {
                this->acceptor.accept(socket);
            }

            std::array<char, N> buf;
            asio::error_code error;
            size_t len = this->socket.read_some(asio::buffer(buf), error);

            if(error)
                throw asio::system_error(error);

            std::copy(buf.begin(), buf.end(), std::back_inserter(message));

        } catch(asio::error_code& e) {
            std::cout << e.message() << std::endl;
            return "-1";
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            return "-1";
        }
        return message;
    }

    void inline stop() {
        asio::error_code error;
        socket.shutdown(tcp::socket::shutdown_type::shutdown_send, error);
    }
};
