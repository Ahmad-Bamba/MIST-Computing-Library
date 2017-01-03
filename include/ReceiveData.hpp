#pragma once

#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <asio.hpp>

using asio::ip::tcp;

typedef unsigned short ushort;

namespace MIST {
    class ReceiveData {
        //RUN TWICE
        //First pull out <16> bytes, then parse that to figure out how much more to read after that, then read again
    private:
        asio::io_service service;
        tcp::acceptor acceptor;
        tcp::socket socket;
    public:
        ReceiveData(ushort port = 8008) : acceptor(service, tcp::endpoint(tcp::v4(), port)),
                                   socket(service) { }

        template<size_t N>
        inline std::string receive() {
            std::string message;
            try {
                if (!socket.is_open())
                {
                    acceptor.accept(socket);
                }
                
                std::array<char, N> buf;
                asio::error_code error;
                size_t len = socket.read_some(asio::buffer(buf), error);

                if(error)
                    throw asio::system_error(error);

                //std::cout.write(buf.data(), len);
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

        void stop()
        {
            asio::error_code error;
            socket.shutdown(tcp::socket::shutdown_type::shutdown_send, error);
        }
    };
}
