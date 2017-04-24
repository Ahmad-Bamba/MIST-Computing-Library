#pragma once

#include <string>
#include <iostream>
#include <tuple>
#include <cassert>
#include <sstream>

#ifndef WIN32

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

class SendData {
private:
    unsigned short m_port;
    std::string m_ip;
    int m_sock;
    sockaddr_in m_address;

    inline bool send(const char* raw, size_t size) {
        const char* buffer = new char[size];
        buffer = raw;
        if(write(m_sock, buffer, size + 1) < 0) {
            std::cerr << "Send.h: Failed to write data to socket at line " << __LINE__ << std::endl;
            return true;
        }
        return false;
    }
public:
    SendData(const std::string& ip, const unsigned short& port) {
        m_port = port;
        m_ip = ip;

        m_sock = socket(AF_INET, SOCK_STREAM, 0);

        std::stringstream ss;
        ss << "Send.h: Error opening socket at line " << __LINE__ << std::endl;
        assert(m_sock >= 0 && ss.str().c_str());

        m_address.sin_family = AF_INET;
        m_address.sin_port = htons(m_port);
        
        if(inet_pton(AF_INET, m_ip.c_str(), &m_address.sin_addr))
            std::cerr << "Send.h Error getting set hostname (inet_pton) at line " << __LINE__ << std::endl;
    }

    SendData(const SendData& from) = delete;
    SendData(SendData&& move) = delete;

    inline bool establish() {
        if(connect(m_sock, (struct sockaddr *)&m_address, sizeof(m_address)) < 0) {
            std::cerr << "Send.h: Error connecting socket at line " << __LINE__ << std::endl;
            return false;
        }
        return true;
    }

    inline bool send_string(std::string data) {
        return send(data.c_str(), data.size());
    }

    inline auto get_info() {
        return std::tuple<unsigned short, std::string, int>(m_port, m_ip, m_sock);
    }
};

#else

class SendData {
private:
    unsigned short m_port;
    std::string m_ip;

    inline bool send(const char* raw, size_t size) {
        const char* buffer = new char[size];
        return false;
    }
public:
    SendData(const std::string& ip, const unsigned short& port) {
        m_port = port;
        m_ip = ip;
    }

    SendData(const SendData& from) = delete;
    SendData(SendData&& move) = delete;

    inline bool establish() {
        return false;
    }

    inline bool send_string(std::string data) {
        return send(data.c_str(), data.size());
    }

    inline auto get_info() {
        return std::tuple<unsigned short, std::string, int>(m_port, "", 0);
    }
};


#endif // WIN32
