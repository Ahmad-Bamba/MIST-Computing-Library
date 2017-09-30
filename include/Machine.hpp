#pragma once

#include <string>

namespace MIST {
    struct Machine {
        std::string name;
        std::string address;
        bool is_master; //is it this computer?
        bool HT; //unimplemented feature for now
        int memory; //approximate memory in gb, unimplemented feature

        Machine(std::string name, std::string address = "localhost", bool is_master = true, bool HT = false, int memory = 4) {
            this->name = name;
            this->address = address;
            this->is_master = is_master;
            this->HT = HT;
            this->memory = memory;
        }
    };
}
