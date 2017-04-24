#pragma once

#include <string>
#include <iostream>
#include <tuple>

#ifndef WIN32

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class ReceiveData {
private:
    unsigned short m_port;
    int m_sock;
    sockaddr_in m_address, m_client;
public:
    ReceiveData(const unsigned short& port) {
        m_port = port;

        m_sock = socket(AF_INET, SOCK_STREAM, 0);
        m_address.sin_family = AF_INET;
        m_address.sin_port = htons(m_port);
        m_address.sin_addr.s_addr = INADDR_ANY;

        if(m_sock < 0)
            std::cerr << "Receive.h: Failed to open socket at line " << __LINE__ << std::endl;
    }

    ReceiveData(const ReceiveData& from) = delete;
    ReceiveData(ReceiveData&& move) = delete;

    inline bool establish() {
        if (bind(m_sock, (struct sockaddr *) &m_address, sizeof(m_address)) < 0) {
            std::cerr << "Receive.h: failed to bind port at line " << __LINE__ << std::endl;
            return false;
        }
        listen(m_sock, 5);
        return true;
    }

    inline std::string receive_chunk(size_t size) {
        char* buffer = new char[size];

        unsigned int len = sizeof(m_client);
        int clientfd = accept(m_sock, (struct sockaddr*)(&m_client), &len);
        if(clientfd < 0) {
            std::cerr << "Receive.h: Error accepting socket at line " << __LINE__ << std::endl;
            return "";
        }
        recv(clientfd, buffer, size, MSG_WAITALL);
        std::string s = std::string(buffer);
        delete buffer;
        return s;
    }

    inline auto get_info() {
        return std::tuple<unsigned short, int>(m_port, m_sock);
    }
};

#else

class ReceiveData() {
public:
    ReceiveData() = default;
    ReceiveData(const ReceiveData& from) = delete;
    ReceiveData(ReceiveData&& move) = delete;
};

#endif // WIN32
