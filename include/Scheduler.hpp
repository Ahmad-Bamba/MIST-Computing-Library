#pragma once

#include <networking/Receive.hpp>
#include <networking/Send.hpp>
#include <MIST.pb.h>
#include <Task.hpp>
#include <vector>
#include <memory>
#include <thread>
#include <Machine.hpp>

class Scheduler {
private:
    std::vector<MIST::TaskInterface*> task_queue;
    bool running = false;
    std::thread* checker;
public:
    Scheduler(std::vector<MIST::TaskInterface*> task_queue = {}) {
        this->task_queue = task_queue;
        start();
    }

    ~Scheduler() {
        //Make sure threads are close
        stop();
        //empty task_queue to prevent evil raw pointers from ruining program
        task_queue.empty();
    }

    template<typename ... P>
    inline void update_task_vector(std::string id, std::function<void(P...)> fn, std::tuple<P...> args) {
        task_queue.push_back(new MIST::Task<P...>(id, fn, args));
    }

    inline void remove_task(std::string id) {
        std::vector<MIST::TaskInterface*> copy = {};
        for(auto t : task_queue) {
            if(t->id != id) {
                copy.push_back(t);
            }
        }
        task_queue = copy;
    }

    inline void check_for_tasks() {
        while(this->running) {
            auto rdo = std::make_shared<ReceiveData>(8008);
            bool end = false;
            std::string message = "";
            auto parsed = std::make_shared<ProtobufMIST::Task>();
            rdo->establish();

            while(!end) {
                std::string x = rdo->receive_chunk(1);
                if( (x.find((char)185) != std::string::npos) || x == "") {
                    end = true;
                } else {
                    message += x;
                }
            }

            if(parsed->ParseFromString(message)) {
                for(auto task : this->task_queue) {
                    if(parsed->task_name() == task->id) {
                        auto t = std::make_shared<std::thread>(&MIST::TaskInterface::run, task);
                        t->join();
                        t = nullptr;
                    }
                }
            } else {
                std::cout << "Warning: Could not parse message as task. "
                             "This does not necessarily mean something has gone wrong.\n";
            }
            rdo = nullptr;
            parsed = nullptr;
        }
    }

    inline void start() {
        this->running = true;
        this->checker = new std::thread(&Scheduler::check_for_tasks, this);
    }

    //run in new thread
    inline void run_task(std::string id) {
        for(auto task : this->task_queue) {
            if(id == task->id) {
                task->run();
            }
        }
    }

    //run all specified tasks concurrently
    inline void run_task(std::vector<std::string> ids) {
        std::vector<std::thread*> threads;
        for(auto id : ids) {
            for(auto task : task_queue) {
                if(task->id == id) {
                    threads.push_back(new std::thread(&MIST::TaskInterface::run, task));
                }
            }
        }

        for(auto thread : threads) {
            thread->join();
        }

        threads.empty();
    }

    inline void send_task(std::string task, MIST::Machine machine, short int port) {
        SendData* sd = new SendData(machine.address, port);
        sd->establish();
        task += (char)182;
        sd->send_string(task);
        delete sd;
    }

    inline void stop() {
        this->running = false;
        delete checker;
    }
};
