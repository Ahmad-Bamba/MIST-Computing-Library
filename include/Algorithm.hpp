#pragma once

#include <vector>
#include <tuple>
#include <string>

namespace MIST {
    enum class TaskRunType {
        SYNC,
        ASYNC,
    };

    enum class AlgorithmMode {
        SOLO,
        EVEN,
        MEM_BIAS,
        THR_BIAS,
    };

    class Algorithm {
    protected:
        std::vector<std::tuple<std::string, TaskRunType>> m_to_run;
    public:
        Algorithm() = default;
        void add_sequential(std::string name) {
            m_to_run.push_back(std::make_tuple(name, TaskRunType::SYNC));
        }

        void add_parallel(std::string name) {
            m_to_run.push_back(std::make_tuple(name, TaskRunType::ASYNC));
        }

        auto get_tasks() { return m_to_run; }
    };
}
