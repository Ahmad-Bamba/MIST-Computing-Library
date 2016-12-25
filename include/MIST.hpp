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
    	    bool enableHT;
    	};

    	struct MISTHost
    	{
            std::string name;
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
            local.isLocal = true;
            local.address = "local";
            local.computer.allowedMemory = memory;
            local.computer.allowedThreads = threads;
            local.name = name;
            local.computer.enableHT = enableHT;
		}
		void setThreads(unsigned long threads) {
            local.computer.allowedThreads = threads;
		}
		void setMemory(unsigned long long memory) {
            local.computer.allowedMemory = memory;
		}
		void enableHT(bool enableHT) {
            local.computer.enableHT = enableHT;
		}
		void setName(std::string computerName) {
            local.name = computerName;
		}
		void setAddress(std::string address) {
            local.address = address;
		}

		//NETWORK INITIALIZATION
        void inviteMachine(std::string IP) { //invites machine to network (changed from addMachine to signify that only master machine can do this)
            //Perhaps accomplish this by creating an invite task in MIST.hpp that sends information back to this machine?
        }

  /*    void sendTask(std::shared_ptr<Task<auto>> task) {
           send(task.getID());
        }*/

    };
}
