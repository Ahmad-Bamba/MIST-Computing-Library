#pragma once

#include "Scheduler.hpp"
#include "Task.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <Machine.hpp>

#define MIST_DEFAULT_PORT 8008

namespace MIST {
    class MIST {
    private:
    	std::unique_ptr<Machine> local;
        std::shared_ptr<Scheduler> scheduler;
        std::vector<Machine> machines;
        bool is_master; //unimplemented for now
    public:
        MIST(bool is_master, Machine local) noexcept {
            scheduler = std::make_shared<Scheduler>();
            machines.push_back(local);
            this->is_master = is_master;
            this->local.reset(&local);
        }

        MIST(bool is_master, std::vector<Machine> machines, int local_index = 0) noexcept {
            scheduler = std::make_shared<Scheduler>();
            this->machines = machines;
            this->is_master = is_master;
            local.reset(&this->machines[local_index]);
        }
        MIST(const MIST&) = delete;
        MIST operator=(const MIST&) = delete;
        ~MIST() noexcept = default;

        void add_machine(Machine machine) {
            this->machines.push_back(machine);
        }

        void remove_machine(Machine* to_remove) {
            std::vector<Machine> v(this->machines.size() - 1);
            std::copy_if(this->machines.begin(), this->machines.end(), v.end(), [&to_remove](Machine* m) {
                return to_remove != m;
            });
            machines = v;
        }

        void remove_machine(std::string name) {
            Machine* to_remove = nullptr;
            for (auto& machine : machines) {
                if (machine.name == name) {
                    to_remove = &machine;
                    break;
                }
            }
            remove_machine(to_remove);
        }

        const std::unique_ptr<Machine>& get_local() { 
            return local; 
        }

        template<typename ... P>
        void add_task(std::string id, std::function<void(std::tuple<P...>)> fn, std::tuple<P...> args) {
            scheduler->update_task_vector(id, fn, args);
        }

        void send_task(std::string serialized_task, std::string machine_name, short int port = MIST_DEFAULT_PORT) {
            Machine* other = nullptr;
            for(auto& machine : machines) {
                if(machine.name == machine_name) {
                    other = &machine;
                    break;
                }
            }
            send_task(serialized_task, other, port);
        }

        void send_task(std::string serialized_task, Machine* other, short int port = MIST_DEFAULT_PORT) {
            if(other != nullptr)
                scheduler->send_task(serialized_task, *other, port);
        }

        const std::vector<Machine>& get_machines() { 
            return machines;
        }
    };
}
