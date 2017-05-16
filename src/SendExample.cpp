#include <iostream>
#include <networking/Send.hpp>


int main() {
    std::cout << "Sending data to localhost..." << std::endl;
	SendData s { "localhost", 8008 };
    s.establish();
    s.send_string("lolololo");
    std::cout << "Sent!" << std::endl;
	std::cout << "Socket info: " << std::get<SEND_PORT>(s.get_info()) << " " << std::get<SEND_IP>(s.get_info()) << " " << std::get<SEND_SOCK>(s.get_info()) << std::endl;
}
