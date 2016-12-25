// This file exists to facilitate testing

#include <asio.hpp>
#include <Task.hpp>
#include <MIST.hpp>
#include <ReceiveData.hpp>

int main() {
    MIST::ReceiveData receiveObj;

    std::string s = receiveObj.receive<64>();
    std::cout << "Message received: '" << s << "'" << std::endl;

    return 0;
}
