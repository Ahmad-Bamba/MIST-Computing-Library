#pragma once

#include <string>

namespace MIST {
    struct Machine {
        std::string name;
        std::string address;
        bool isLocal; //is it this computer?
        bool HT; //unimplemented feature for now
        int memory; //approximate memory in gb, unimplemented feature

        Machine(std::string name, std::string address = "localhost", bool isLocal = true, bool HT = false, int memory = 4) {
            this->name = name;
            this->address = address;
            this->isLocal = isLocal;
            this->HT = HT;
            this->memory = memory;
        }
    };
}
