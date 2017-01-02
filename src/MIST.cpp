// This file exists to facilitate testing

//#include <asio.hpp>
#include <Task.hpp> //user defined, not relevant
#include <MIST.hpp> //user defined, not relevant
#include <ReceiveData.hpp> //user defined, not relevant
#include <MIST.pb.h> //protobuf file made from protoc

int main() {
    /*MIST::ReceiveData receiveObj;

    std::string s = receiveObj.receive<64>();
    std::cout << "Message received: '" << s << "'" << std::endl;*/

    ProtobufMIST::Task taskObj;
    taskObj.set_task_name("The Best Task");
    taskObj.set_task_id("7");

    std::string* message;
    if(!taskObj.SerializeToString(message))
        std::cout << "Task '" << taskObj.task_name() << "'" << " not serialized successfully\n";
    else
        std::cout << message << std::endl;

    return 0;
}
