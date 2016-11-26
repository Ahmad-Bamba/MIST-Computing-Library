#pragma once

#include <MIST_Internal.hpp>
#include <Scheduler.hpp>
#include <Task.hpp>
#include <string>
#include <vector>
#include <memory>
#include <asio.hpp>

namespace MIST {
    class MIST {
    private:
        std::vector<std::string> IPs;
        std::shared_ptr<Scheduler> scheduler;
        bool is_master;

        void send(auto message) {
            //send message using ASIO
        }

    public:
        MIST(std::vector<std::string> IPs, bool is_master) {
            scheduler = std::make_shared<Scheduler>();
            this->IPs = IPs;
            this->is_master = is_master;
        }

        ~MIST() = default;

        void addMachine(std::string IP) {
            IPs.push_back(IP);
        }

        void sendTask(std::shared_ptr<Task<auto>> task) {
            send(task.getID());
        }
    };
}
