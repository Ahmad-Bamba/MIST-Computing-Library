#pragma once

#include <MIST_Internal.hpp>
#include <Task.hpp>
#include <vector>
#include <memory>
#include <thread>

class Scheduler {
private:
    std::vector<Task> task_queue;
    bool running = false;

public:
    Scheduler() {

    }

    virtual ~Scheduler() = 0;

    void updateTaskVector(std::string id, MIST_taskfunc fn) {
        task_queue.push_back(std::make_shared<Task>(id, fn));
    }

    void removeTask(std::string id) {
        std::vector<Task> copy;
        for(auto t : task_queue) {
            if(t.getID() != id) {
                copy.push_back(t);
            }
        }
        task_queue = copy;
    }

    void Start();
    void Stop();
};
