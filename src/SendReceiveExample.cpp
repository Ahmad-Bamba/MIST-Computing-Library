#define SEND

#ifdef SEND
#include "../include/networking/Send.hpp"
#else
#include "../include/networking/Receive.hpp"
#endif

#include <iostream>
#include <algorithm>

int main() {
#ifdef SEND
    std::cout << "Sending data to localhost..." << std::endl;
	SendData s { "localhost", 8008 };
    s.establish();
    s.send_string("lolololo");
    std::cout << "Sent!" << std::endl;
	std::cout << "Socket info: " << std::get<SEND_PORT>(s.get_info()) << " " << std::get<SEND_IP>(s.get_info()) << " " << std::get<SEND_SOCK>(s.get_info()) << std::endl;
#else
    std::cout << "Receiving from 8008" << std::endl;
	ReceiveData r { 8008 };
    r.establish();
    auto x = r.receive_chunk(4);
    std::cout << "Got: " << x << std::endl;
	std::cout << "Socket info: " << std::get<RECEIVE_PORT>(r.get_info()) << " " << std::get<RECEIVE_SOCK>(r.get_info());
#endif
}
