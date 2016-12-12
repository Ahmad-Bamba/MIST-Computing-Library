#pragma once

#include <MIST_Internal.hpp>
#include <Task.hpp>
#include <vector>
#include <memory>
#include <thread>

class Scheduler {
private:
    std::vector<MIST::Task*> task_queue;
    bool running = false;
    std::thread* checker;

public:
    Scheduler(std::vector<MIST::Task*> task_queue = {}) {
        this->task_queue = task_queue;
        Start();
    }

    //virtual ~Scheduler() = 0;

    void updateTaskVector(std::string id, MIST_taskfunc fn) {
        task_queue.push_back(new MIST::Task(id, fn));
    }

    void removeTask(std::string id) {
        std::vector<MIST::Task*> copy = {};
        for(auto t : task_queue) {
            if(t->getID() != id) {
                copy.push_back(t);
            }
        }
        task_queue = copy;
    }

    void check_for_tasks() {
        while(this->running) {
            // It's time for pseudocode.
            /*
             * message = socket.get
             * found = len(message) > 0 ? true : false
             * if found:
             *      if id_valid(message):
             *          for task in task_queue:
             *          if task.id == message:
             *              t = std::thread(task.run)
             *              t.join()
             */
        }
    }

    void Start() {
        this->running = true;
        this->checker = new std::thread(&Scheduler::check_for_tasks, this);
    }

    void Stop() {
        delete checker;
    }
};
