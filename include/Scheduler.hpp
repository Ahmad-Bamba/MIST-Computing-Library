#pragma once

#include <networking/Receive.hpp>
#include <networking/Send.hpp>
#include <MIST.pb.h>
#include <Task.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <Machine.hpp>

#define MIST_DEFAULT_PORT 8008

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

    ~Scheduler() noexcept {
        //Make sure threads are close
        stop();
        //empty task_queue to prevent evil raw pointers from ruining program
        task_queue.empty();
    }

    template<typename ... P>
    void update_task_vector(std::string id, std::function<void(std::tuple<P...>)> fn, std::tuple<P...> args) {
        task_queue.push_back(new MIST::Task<P...>(id, fn, args));
    }

    void remove_task(std::string id) {
        MIST::TaskInterface* to_remove = nullptr;
        for (auto& task : task_queue) {
            if (task->id == id) {
                to_remove = task;
                break;
            }
        }
        remove_task(to_remove);
    }

    void remove_task(MIST::TaskInterface* to_remove) {
        std::vector<MIST::TaskInterface*> copy(task_queue.size() - 1);
        std::copy_if(task_queue.begin(), task_queue.end(), copy.end(), [&to_remove](MIST::TaskInterface* t) { 
            return to_remove != t;
        });
        task_queue = copy;
    }

    void check_for_tasks() {
        while(this->running) {
            ReceiveData rdo(MIST_DEFAULT_PORT);
            bool end = false;
            std::string message = "";
            ProtobufMIST::Task parsed;
            rdo.establish();

            while(!end) {
                std::string x = rdo.receive_chunk(1);
                if( (x.find((char)185) != std::string::npos) || x == "") {
                    end = true;
                } else {
                    message += x;
                }
            }

            if(parsed.ParseFromString(message)) {
                for(const auto& task : this->task_queue) {
                    if(parsed.task_name() == task->id) {
                        std::thread t = std::thread(&MIST::TaskInterface::run, task);
                        t.join();
                        break;
                    }
                }
            } /*else {
                std::cout << "Warning: Could not parse message as task. "
                             "This does not necessarily mean something has gone wrong.\n";
            }*/
        }
    }

    void start() {
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
    void run_task(std::vector<std::string> ids) {
        std::vector<std::thread*> threads;
        for(auto id : ids) {
            for(auto task : task_queue) {
                if(task->id == id) {
                    threads.push_back(new std::thread(&MIST::TaskInterface::run, task));
                }
            }
        }

        for(auto thread : threads)
            thread->join();

        threads.empty();
    }

    void send_task(std::string task, MIST::Machine machine, short int port) {
        SendData sd(machine.address, port);
        sd.establish();
        task += (char)182;
        sd.send_string(task);
    }

    void stop() {
        this->running = false;
        delete checker;
    }
};
