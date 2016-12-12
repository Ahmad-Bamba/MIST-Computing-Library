#pragma once

#include <MIST_Internal.hpp>
#include <string>

/*
 * Tasks are to be defined locally. Pass a function you want to run when an
 * ID is sent over MIST.
 */

namespace MIST {
    class Task {
    private:
        std::string id;
        MIST_taskfunc fn;

    public:
        Task(std::string id, MIST_taskfunc fn) {
            this->id = id;
            this->fn = fn;
        }

		//void send(std::string message);

        /*~Task() {
            delete id;
        }*/

        std::string getID() {
            return id;
        }

        void run() {
            fn();
        }
    };
} /* MIST */
