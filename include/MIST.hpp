#pragma once

#include "Scheduler.hpp"
#include "Task.hpp"
#include "networking/MistAssert.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <Machine.hpp>

#define MIST_DEFAULT_PORT 8008

namespace MIST {
    // main user class
    class MIST {
    private:
    	std::unique_ptr<Machine> m_local;
        std::unique_ptr<Scheduler> m_scheduler;
        std::vector<Machine*> m_machines;
        bool m_is_master;
        std::unique_ptr<Algorithm> m_alg;
        AlgorithmMode m_mode;
    public:
        MIST(Machine* local, AlgorithmMode mode, Algorithm* alg) noexcept {
            m_machines.push_back(local);
            m_is_master = local->is_master;
            m_local.reset(local);
            m_alg.reset(alg);
            m_mode = mode;

            // if this is the only machine, there is no need to check for incoming
            // tasks on the network
            if (m_mode == AlgorithmMode::SOLO)
                m_scheduler = std::make_unique<Scheduler>();
            else
                m_scheduler = std::make_unique<Scheduler>(true);
        }

        MIST(std::vector<Machine*> machines, int local_index, AlgorithmMode mode, Algorithm* alg) noexcept {
            m_scheduler = std::make_unique<Scheduler>();
            m_machines = machines;
            m_local.reset(m_machines[local_index]);
            m_is_master = m_local->is_master;
            m_alg.reset(alg);
            m_mode = mode;
        }
        // no copy pls
        MIST(const MIST&) = delete;
        MIST operator=(const MIST&) = delete;
        // no move pls
        MIST(MIST&&) = delete;

        // prevent memory leak
        ~MIST() noexcept {
            for (auto it = m_machines.begin(); it != m_machines.end(); it++)
                delete (*it);
            m_machines.empty();
        }

        int run() {
            int status = 0;
            switch (m_mode) {
            case AlgorithmMode::SOLO:
                status = m_scheduler->run_tasks(m_alg->get_tasks());
                break;
            case AlgorithmMode::EVEN:
                // todo
                break;
            case AlgorithmMode::MEM_BIAS:
                // todo
                break;
            case AlgorithmMode::THR_BIAS:
                //todo
                break;
            }
            return status;
        }

        void add_machine(Machine* machine) {
            m_machines.push_back(machine);
        }

        void remove_machine(Machine* to_remove) {
            std::vector<Machine*> v(m_machines.size() - 1);
            std::copy_if(this->m_machines.begin(), this->m_machines.end(), v.end(), [&to_remove](Machine* m) {
                return to_remove != m;
            });
            m_machines = v;
        }

        void remove_machine(std::string name) {
            Machine* to_remove = nullptr;
            for (auto& machine : m_machines) {
                if (machine->name == name) {
                    to_remove = machine;
                    break;
                }
            }
            remove_machine(to_remove);
        }

        const std::unique_ptr<Machine>& get_local() {
            return m_local; 
        }

        template<typename ... P>
        void add_task(std::string id, std::function<void(std::tuple<P...>)> fn, std::tuple<P...> args) {
            m_scheduler->update_task_vector(id, new Task<P ...>(fn, args));
        }

        const auto& get_machines() { 
            return m_machines;
        }
    };
}
