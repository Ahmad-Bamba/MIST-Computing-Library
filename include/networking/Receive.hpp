#pragma once

#include <string>
#include <iostream>
#include <tuple>
#include <cassert>
#include <sstream>

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

        std::stringstream ss;
        ss << "Receive.h: Failed to open socket at line " << __LINE__ << std::endl;
        assert(m_sock >= 0 && ss.str().c_str());
    }

    ReceiveData(const ReceiveData& from) = delete;
    ReceiveData(ReceiveData&& move) = delete;

    inline bool establish() {
        if (bind(m_sock, (struct sockaddr *) &m_address, sizeof(m_address)) < 0) {
            std::cerr << "Receive.h: failed to bind port at line " << __LINE__ << std::endl;
            return true;
        }
        listen(m_sock, 5);
        return false;
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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <memory>
#include <exception>
#include <string>

class ReceiveData {
    short m_port;
    std::unique_ptr<WSADATA> m_wsa_data { nullptr };
    // The pointer ruse was a distaction
    addrinfo* m_result;
    std::unique_ptr<addrinfo> m_ptr { nullptr };
    addrinfo m_hints { }; //TODO make sure this actually works
    SOCKET m_sock;
public:
    ReceiveData(const unsigned short& port) {
        m_port = port;

        std::stringstream ss;
        ss << "Receive.h: Failed to open socket at line " << __LINE__ << std::endl;
        assert(WSAStartup(MAKEWORD(2, 2), m_wsa_data.get()) != 0 && ss.str().c_str());

        m_hints.ai_family = AF_INET;
        m_hints.ai_socktype = SOCK_STREAM;
        m_hints.ai_protocol = IPPROTO_TCP;
        m_hints.ai_flags = AI_PASSIVE;

        if(getaddrinfo(nullptr, std::to_string(m_port).c_str(), &m_hints, &m_result) != 0) {
            std::cerr << "getadderinfo failed at line " << __LINE__ << std::endl;
            WSACleanup();
            std::terminate();
        }

        m_sock = INVALID_SOCKET;
        m_sock = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
    }
    ReceiveData(const ReceiveData& from) = delete;
    ReceiveData(ReceiveData&& move) = delete;
    ~ReceiveData() {
        if(m_result != nullptr)
            delete m_result;
        WSACleanup();
    }

    inline bool establish() {
        if(bind(m_sock, m_result->ai_addr, static_cast<int>(m_result->ai_addrlen)) == SOCKET_ERROR) {
            std::cerr << "Receive.h: bind failed with error " << WSAGetLastError() << " at line " << __LINE__ << std::endl;
            closesocket(m_sock);
            return true;
        }
        if (listen(m_sock, 5) == SOCKET_ERROR) {
            std::cerr << "Receive.h: listen failed with error " << WSAGetLastError() << " at line " << __LINE__ << std::endl;
            closesocket(m_sock);
            return true;
        }
        delete m_result;
        return false;
    }

    inline std::string receive_chunk(size_t size) {
        char* buffer = new char[size];
        SOCKET client;
        client = accept(client, nullptr, nullptr);
        if(client == INVALID_SOCKET) {
            std::cerr << "Receive.h: accept failed with error " << WSAGetLastError() << " at line " << __LINE__ << std::endl;
            closesocket(m_sock);
            return "";
        }
        int i;
        std::string ret;
        //accept until connection closes
        do {
            i = recv(client, buffer, size, 0);
            if(i > 0)
                ret = std::string(buffer);
        } while(i > 0);
        return ret;
    }

    inline auto get_info() {
        return std::tuple<unsigned short, int>(m_port, m_sock);
    }
};

#endif // WIN32
