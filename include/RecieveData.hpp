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
        //bool server;
    public:
        RecieveData(std::string ip = "localhost") {
            this->ip = ip;
        }

        void read() {

        }
    };
}
