/*// This file exists to facilitate testing
#include "../protobuf_files/MIST.pb.h"
#include <asio.hpp>
#include <Task.hpp>
#include <iostream>
#include <MIST.hpp>
#include <ReceiveData.hpp>

int main() {
    MIST::ReceiveData receiveObj;

    std::string s = receiveObj.receive<8>();
    std::cout << s << std::endl;
    bool end_seq_found = false;
    while(!end_seq_found)
    {
        s.append(receiveObj.receive<4>());
        if (s.substr(s.length()-4, 4) == "^^^^")
        {
            std::cout << "This string is " << s.length() << " long." << std::endl; //debugging
            s.erase(s.end() - 4, s.end());
            end_seq_found = true;
        }
    }

    for (int i = 0; i++; i == 3)
    {
        if (s.at(s.length()) == '^')
        {
            s.pop_back();
        }
        else {
            i = 3;
        }
    }
    std::cout << s << std::endl;
    std::string id = s.substr(0, 8);
    //do stuff
    std::cout << s.length();//debugging
    s.erase(0, 8);
    std::cout << s.length(); //debugging
    std::cout << id << std::endl;//debugging
    ProtobufMIST::ComputerInformation c;
    if (c.ParseFromString(s))
    {
        std::cout << c.address() << std::endl;
    }else{
        std::cout << "Failure" << std::endl;
    }
    //std::cout << "Message received: '" << s << "'" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    receiveObj.stop();

    return 0;
}
*/