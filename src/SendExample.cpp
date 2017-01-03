// Example sending file for future reference

#include <asio.hpp>
#include <Task.hpp>
#include <MIST.hpp>
#include <SendData.hpp>
#include <MIST.pb.h>

const char delimiter = 182; // paragraph sign

int main() {
    MIST::SendData sendObj("localhost", 8008);
    ProtobufMIST::Task taskObj;
    taskObj.set_task_name("Puppy");
    taskObj.set_task_id("0");
    std::string task_string;
    taskObj.SerializeToString(&task_string);
    task_string.append(&delimiter);
    std::cout << "This is very dumb: " << task_string << std::endl;
    sendObj.simple_send(task_string.c_str());

    return 0;
}
