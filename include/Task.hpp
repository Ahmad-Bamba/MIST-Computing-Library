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
        std::string id;

        TaskInterface(std::string id) {
            this->id = id;
        }
    };

    template<class ... P>
    struct Task: public TaskInterface {
        Task(std::string id, std::function<std::tuple<P...>> fn, std::tuple<P...> args) : TaskInterface(id) {
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
