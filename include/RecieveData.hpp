#pragma once

#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <asio.hpp>

using asio::ip::tcp;

namespace MIST {
    class RecieveData {
    private:
        std::string ip;

        asio::io_service service;
        tcp::resolver resolver;
        tcp::resolver::iterator iterator;
        tcp::resolver::query query;
        tcp::socket socket;
        //bool server;
    public:
        RecieveData(std::string ip = "localhost") {
            this->ip = ip;

            resolver = tcp::resolver(service);
            query = tcp::resolver::query(tcp::v4(), ip, "8008");
            iterator = resolver.resolve(query);
            socket = tcp::socket(service);
        }

        inline std::string receive(size_t size) {
            //try to read the socket in a certain size message
            std::string message;
            try {
                asio::connect(socket, iterator);
                std::array<char, size> buff;
                asio::error_code error;
                size_t len = socket.read_some(asio::buffer(buff), error);

                if(error == asio::error::eof) {
                    return "0"; //connection broken cleanly
                } else if(error) {
                    throw asio::system_error(error); //some other error
                }

                //std::cout.write(buff.data(), len);
                //set message to whatever was sent
                std::copy(buff.begin(), buff.begin() + static_cast<int>(size), std::back_inserter(message));
            }catch(std::exception& e) {
                std::cout << e.what() << std::endl;
                return "-1";
            }
            return received;
        }
    };
}
