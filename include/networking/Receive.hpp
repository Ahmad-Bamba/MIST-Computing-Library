#pragma once

#include <string>
#include <iostream>
#include <tuple>
#include <sstream>
#include "MistAssert.hpp"

#define MAX_LISTEN_CONNECTIONS 5
#define RECEIVE_PORT 0
#define RECEIVE_SOCK 1

#ifndef _WIN32

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

        mistassert(m_sock >= 0, "Receive.hpp: failed to open socket!");
    }

    bool establish() {
        if (bind(m_sock, (struct sockaddr *) &m_address, sizeof(m_address)) < 0) {
            std::cerr << "Receive.h: failed to bind port at line " << __LINE__ << std::endl;
            return true;
        }
        listen(m_sock, 5);
        return false;
    }

    std::string receive_chunk(size_t size) {
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

    auto get_info() {
        return std::tuple<unsigned short, int>(m_port, m_sock);
    }
};

#else

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

#include <memory>
#include <string>

// TODO: connection actively refused
class ReceiveData {
    unsigned short m_port;
    const char* m_port_str;
    SOCKET m_sock;
    WSADATA m_wsa_data;
    addrinfo* m_result { nullptr };
    addrinfo* m_ptr { nullptr };
    addrinfo m_hints;
public:
    ReceiveData(const unsigned short& port) {
        m_port = port;
        m_port_str = std::to_string(m_port).c_str();

        int err = WSAStartup(MAKEWORD(2, 2), &m_wsa_data);
        char* err_buf = new char[252];
        sprintf(err_buf, "Error %d on WSAStartup!", err);
        mistassert(err == 0, err_buf);

        ZeroMemory(&m_hints, sizeof(m_hints));
        m_hints.ai_family = AF_INET;
        m_hints.ai_socktype = SOCK_STREAM;
        m_hints.ai_protocol = IPPROTO_TCP;
        m_hints.ai_flags = AI_PASSIVE;

        int result = getaddrinfo(NULL, m_port_str, &m_hints, &m_result);
        char* res_buf = new char[252];
        sprintf(res_buf, "Error %d on getaddrinfo!", result);
        mistassert(result == 0, res_buf);
    }

    bool establish() {
        m_sock = INVALID_SOCKET;
        m_sock = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
        if (m_sock == INVALID_SOCKET) {
            std::cerr << "Invalid socket on line " << __LINE__ << " of Receive.hpp" << std::endl;
            freeaddrinfo(m_result);
            WSACleanup();
            return true;
        }

        int result = bind(m_sock, m_result->ai_addr, static_cast<int>(m_result->ai_addrlen));
        if (result == SOCKET_ERROR) {
            std::cerr << "Receive.hpp: Failed to bind to socket at line " << __LINE__ << std::endl;
            freeaddrinfo(m_result);
            closesocket(m_sock);
            WSACleanup();
            return true;
        }

        if (listen(m_sock, MAX_LISTEN_CONNECTIONS) == SOCKET_ERROR) {
            std::cerr << "Receive.hpp: Listen failed with error: " << WSAGetLastError() << std::endl;
            closesocket(m_sock);
            WSACleanup();
            return true;
        }
        return false;
    }

    std::string receive_chunk(size_t size) {
        SOCKET cli;
        cli = INVALID_SOCKET;
        cli = accept(m_sock, NULL, NULL);
        if (cli == INVALID_SOCKET) {
            std::cerr << "Receive.hpp: Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(m_sock);
            WSACleanup();
            return "";
        }

        char* buf = new char[size];
        int result;
        do {
            result = recv(cli, buf, static_cast<int>(size), 0);
            if (result < 0) {
                std::cerr << "Receive.hpp: Receive failed! " << WSAGetLastError() << std::endl;
                closesocket(cli);
                WSACleanup();
                buf = "";
            }
        } while (result > 0);

        result = shutdown(cli, SD_SEND);
        if (result == SOCKET_ERROR) {
            std::cerr << "Receive.hpp: Unclean disconnect! " << WSAGetLastError() << std::endl;
            closesocket(cli);
            WSACleanup();
        }

        return std::string(buf);
    }

    auto get_info() {
        return std::tuple<unsigned short, int>(m_port, m_sock);
    }
};

#endif // _WIN32
