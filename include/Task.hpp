#pragma once

#include <string>
#include <functional>
#include <tuple>

/*
 * Tasks are to be defined locally. Pass a function you want to run when an
 * ID is sent over MIST.
 */

namespace MIST {
    struct TaskInterface {
        virtual void run() = 0;
    };

    template<class ... P>
    struct Task: public TaskInterface {
        Task(std::function<void(std::tuple<P...>)> fn, std::tuple<P...> args) : TaskInterface() {
            task_params = args;
            task_func = fn;
        };
        void run() override final {
            task_func(task_params);
        }
    private:
        std::function<void(std::tuple<P...>)> task_func;
        std::tuple<P...> task_params;
    };
} /* MIST */
