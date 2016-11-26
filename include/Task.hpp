#pragma once

#include <MIST_Internal.hpp>

/*
 * Tasks are to be defined locally. Pass a function you want to run when an
 * ID is sent over MIST.
 */

namespace MIST {
    template <class T>
    class Task {
    private:
        T id;
        MIST_taskfunc fn;

    public:
        Task(T id, MIST_taskfunc fn) {
            this->id = id;
            this->fn = fn;
        }

		void send(T message);

        ~Task() {
            delete id;
            delete fn;
        }

        auto getID() {
            return id;
        }

        void run() {
            fn();
        }

    };
} /* MIST */
