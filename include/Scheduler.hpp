#pragma once

#include <MIST_Internal.hpp>
#include <networking/ReceiveData.hpp>
#include <networking/SendData.hpp>
#include <MIST.pb.h>
#include <Task.hpp>
#include <vector>
#include <memory>
#include <thread>
#include <Machine.hpp>

class Scheduler {
private:
    std::vector<MIST::Task*> task_queue;
    bool running = false;
    std::thread* checker;
public:
    Scheduler(std::vector<MIST::Task*> task_queue = {}) {
        this->task_queue = task_queue;
        start();
    }

    ~Scheduler() {
        //Make sure threads are close
        stop();
        //empty task_queue to prevent evil raw pointers from ruining program
        task_queue.empty();
    }

    inline void update_task_vector(std::string id, MIST_taskfunc fn) {
        task_queue.push_back(new MIST::Task(id, fn));
    }

    inline void remove_task(std::string id) {
        std::vector<MIST::Task*> copy = {};
        for(auto t : task_queue) {
            if(t->getID() != id) {
                copy.push_back(t);
            }
        }
        task_queue = copy;
    }

    inline void check_for_tasks() {
        while(this->running) {
            auto rdo = std::make_shared<ReceiveData>();
            bool end = false;
            std::string message = "";
            auto parsed = std::make_shared<ProtobufMIST::Task>();

            while(!end) {
                std::string x = rdo->receive<1>();
                if( (x.find(MIST::Internal::delimiter) != std::string::npos) || x == "-1") {
                    end = true;
                } else {
                    message += x;
                }
            }

            if(parsed->ParseFromString(message)) {
                for(auto task : this->task_queue) {
                    if(parsed->task_name() == task->getID()) {
                        auto t = std::make_shared<std::thread>(&MIST::Task::run, task);
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
            if(id == task->getID()) {
                task->run();
            }
        }
    }

    //run all specified tasks concurrently
    inline void run_task(std::vector<std::string> ids) {
        std::vector<std::thread*> threads;
        for(auto id : ids) {
            for(auto task : task_queue) {
                if(task->getID() == id) {
                    threads.push_back(new std::thread(&MIST::Task::run, task));
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
        sd->simple_send(task);
    }

    inline void stop() {
        this->running = false;
        delete checker;
    }
};
