#pragma once

#include "MIST_Internal.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <asio.hpp>

namespace MIST {
    class MIST {
    private:
		struct computerHardware
		{
			unsigned long allowedThreads;
			unsigned long long allowedMemory;
			std::string name;
			bool enableHT;
		};

		struct MISTHost
		{
			bool isLocal; //is it this computer?
			std::string address; //what address is it? "local" for if it's local
			computerHardware computer; //What hardware does it have?
		};

		computerHardware thisComputer; //this computer
		MISTHost local; //this computer

        std::vector<MISTHost> Hosts;
        std::shared_ptr<Scheduler> scheduler;
        bool is_master;

        /*void send(auto message) {
            //send message using ASIO
        }*/

    public:
        MIST(bool is_master) {
            scheduler = std::make_shared<Scheduler>();
            this->is_master = is_master;
        }

        ~MIST() = default;
		//LOCAL INITIALIZATION
		void InitComputer(unsigned long threads = 1, unsigned long long memory = 2048, std::string name = "Unnamed, Unloved Computer", std::string address = "0.0.0.0", bool enableHT = false) {

		}
		void setThreads(unsigned long threads) {

		}
		void setMemory(unsigned long long memory) {

		}
		void enableHT(bool enableHT) {

		}
		void setName(std::string computerName) {

		}
		void setAddress(std::string address) {

		}

		//NETWORK INITIALIZATION
        void addMachine(std::string IP) {
            //IPs.push_back(IP);
        }

  /*      void sendTask(std::shared_ptr<Task<auto>> task) {
            send(task.getID());
        }*/
    };
}
