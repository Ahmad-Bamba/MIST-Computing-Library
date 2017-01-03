// Example receiving file for future reference

#include <asio.hpp>
#include <Task.hpp>
#include <MIST.hpp>
#include <ReceiveData.hpp>
#include <MIST.pb.h>
#include <time.h>
#include <string>

const char delimiter = 182;

void getDate() {
    char date[20];
    time_t now = time(0);
    strftime(date, 20, "%m/%d/%Y %H:%M:%S", localtime(&now));
    std::string date_string(date);
    std::cout << date_string << std::endl;
}

int main() {
    MIST::ReceiveData receiveObj;
    MIST::Task task("Puppy", getDate);
    bool end = false;
    std::string message = "";
    ProtobufMIST::Task parsed;

    int it = 0;
    while(!end) {
        std::string x = receiveObj.receive<1>();
        std::cout << "On it: " << it << " found " << x << std::endl;
        if(/*x == "^^^^" || */x.find(delimiter) != std::string::npos || x.find("-1") != std::string::npos)
            end = true;
        else
            message += x;
        it++;
    }

    std::string cleaned = "";

    for(char c : message) {
        if(c != delimiter) {
            cleaned.push_back(c);
        }
    }
    //cleaned = cleaned.substr(1, cleaned.size() - 2);

    std::cout << "Message: " << cleaned << std::endl;

    if(parsed.ParseFromString(cleaned)) {
        if(task.getID() == parsed.task_name()) {
            auto t = std::make_shared<std::thread>(&MIST::Task::run, task);
            t->join();
            t = nullptr;
        }
    } else {
        std::cout << "Could not parse " << cleaned << std::endl;
    }

    return 0;
}
