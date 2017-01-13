#pragma once

#include "MIST_Internal.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <asio.hpp>
#include <Machine.hpp>

namespace MIST {
    class MIST {
    private:
    	Machine* local;
        std::shared_ptr<Scheduler> scheduler;
        std::vector<Machine> machines;
        bool is_master; //unimplemented for now
    public:
        MIST(bool is_master, Machine local) {
            scheduler = std::make_shared<Scheduler>();
            machines.push_back(local);
            this->is_master = is_master;
            this->local = &local;
        }

        ~MIST() {
            scheduler.reset();
            machines.empty();
            delete local;
        }

        MIST(bool is_master, std::vector<Machine> machines, int local_index = 0) {
            scheduler = std::make_shared<Scheduler>();
            this->machines = machines;
            this->is_master = is_master;
            local = &this->machines[local_index];
        }

        void addMachine(Machine machine) {
            this->machines.push_back(machine);
        }

        void removeMachine(std::string name) {
            std::vector<Machine> v;
            for(auto m : this->machines) {
                if(!(m.name == name)) {
                    v.push_back(m);
                }
            }

            machines = v;
        }

        void addTask(std::string id, MIST_taskfunc fn) {
            scheduler->updateTaskVector(id, fn);
        }

        void sendTask(std::string serialized_task, std::string machine_name, short int port = 8008) {
            for(auto machine : machines) {
                if(machine.name == machine_name) {
                    scheduler->sendTask(serialized_task, machine, port);
                }
            }
        }
    };
}
