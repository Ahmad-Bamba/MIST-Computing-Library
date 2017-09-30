#include <iostream>
#include <networking/Receive.hpp>

int main() {
    std::cout << "Receiving from 8008" << std::endl;
    ReceiveData r { 8008 };
    r.establish();
    auto x = r.receive_chunk(4);
    std::cout << "Got: " << x << std::endl;
    std::cout << "Socket info: " << std::get<RECEIVE_PORT>(r.get_info()) << " " << std::get<RECEIVE_SOCK>(r.get_info());
}
