#pragma once

#include <string>
#include <iostream>
#include <tuple>
#include <sstream>
#include "MistAssert.hpp"

#define SEND_PORT 0
#define SEND_IP   1
#define SEND_SOCK 2

#ifndef _WIN32

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

    bool send(const char* raw, size_t size) {
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

        mistassert(m_sock >= 0, "Send.hpp: Error opening socket!");

        m_address.sin_family = AF_INET;
        m_address.sin_port = htons(m_port);

        mistassert(inet_pton(AF_INET, m_ip.c_str(), &m_address.sin_addr), "Send.hpp: Error getting set hostname (inet_pton)");
    }

    bool establish() {
        if(connect(m_sock, (struct sockaddr *)&m_address, sizeof(m_address)) < 0) {
            std::cerr << "Send.h: Error connecting socket at line " << __LINE__ << std::endl;
            return false;
        }
        return true;
    }

    bool send_string(std::string data) {
        return send(data.c_str(), data.size());
    }

    auto get_info() {
        return std::tuple<unsigned short, std::string, int>(m_port, m_ip, m_sock);
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
#include <exception>
#include <string>

class SendData {
private:
    unsigned short m_port;
    const char* m_port_str;
    std::string m_ip;
    WSADATA m_wsa_data;
    SOCKET m_sock;
    addrinfo* m_result { nullptr };
    addrinfo* m_ptr { nullptr };
    addrinfo m_hints;
public:
    SendData(const std::string& ip, const unsigned short& port) {
        m_ip = ip;
        m_port = port;
        m_port_str = std::to_string(m_port).c_str();

        int err = WSAStartup(MAKEWORD(2, 2), &m_wsa_data);
        char* err_buf = new char[252];
        sprintf(err_buf, "Send.hpp: WSAStartup failed! %d", err);
        mistassert(err == 0, err_buf);

        ZeroMemory(&m_hints, sizeof(m_hints));
        m_hints.ai_family = AF_UNSPEC;
        m_hints.ai_socktype = SOCK_STREAM;
        m_hints.ai_protocol = IPPROTO_TCP;

        int result = getaddrinfo(ip.c_str(), m_port_str, &m_hints, &m_result);
        char* res_buf = new char[252];
        sprintf(res_buf, "Send.hpp: getaddrinfo failed! %d", result);
        mistassert(result == 0, res_buf);
    }

    bool establish() {
        m_sock = INVALID_SOCKET;
        m_ptr = m_result;
        m_sock = socket(m_ptr->ai_family, m_ptr->ai_socktype, m_ptr->ai_protocol);

        if (m_sock == INVALID_SOCKET) {
            std::cerr << "Send.hpp: Error creating socket %ld" << WSAGetLastError() << std::endl;
            freeaddrinfo(m_result);
            WSACleanup();
            return true;
        }

        int result = connect(m_sock, m_ptr->ai_addr, static_cast<int>(m_ptr->ai_addrlen));
        if (result == SOCKET_ERROR) {
            closesocket(m_sock);
            m_sock = INVALID_SOCKET;
        }

        freeaddrinfo(m_result);


        if (m_sock == INVALID_SOCKET) {
            WSACleanup();
            mistassert(false, "Send.hpp: Unable to connect to server!");
        }
        
        return false;
    }

    bool send_string(std::string data) {
        const char* buf = data.c_str();
        int result = send(m_sock, buf, static_cast<int>(data.size()), 0);
        if (result == SOCKET_ERROR) {
            std::cerr << "Send.hpp: send failed " << WSAGetLastError() << std::endl;
            WSACleanup();
            closesocket(m_sock);
            return true;
        }
        return false;
    }

    auto get_info() {
        return std::tuple<unsigned short, std::string, int>(m_port, m_ip, m_sock);
    }
};


#endif // _WIN32
