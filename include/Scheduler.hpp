#pragma once

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <thread>
#include <future>

#include "networking/Receive.hpp"
#include "networking/Send.hpp"
#include "Machine.hpp"
#include "Algorithm.hpp"
#include <MIST.pb.h>
#include "Task.hpp"

#define MIST_DEFAULT_PORT 8008

class Scheduler {
private:
    std::map<std::string, MIST::TaskInterface*> m_task_queue;
    std::vector<std::future<void>> m_async_cleanup;
    bool m_run_checker;
public:
    Scheduler(bool run_checker = false) {
        m_run_checker = run_checker;
        // should offer no overhead if the checker is not enabled
        m_async_cleanup.push_back(std::async(&Scheduler::check_for_tasks, this));
    }
    Scheduler(const Scheduler& from) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(Scheduler const& from) = delete;

    ~Scheduler() noexcept {
        //Make sure threads are close
        stop();
        //empty task_queue to prevent evil raw pointers from ruining program
        /*for (auto it = m_task_queue.begin(); it != m_task_queue.end(); it++)
            delete (*it);*/
        m_task_queue.clear();
    }

    template<typename ... P>
    void update_task_vector(std::string id, MIST::Task<P ...>* t) {
        m_task_queue.insert({ id, t });
    }

    void remove_task(std::string id) {
        auto to_remove = m_task_queue.find(id);
        if (to_remove != m_task_queue.end())
            m_task_queue.erase(to_remove);
    }

    void check_for_tasks() {
        while(m_run_checker) {
            ReceiveData rdo(MIST_DEFAULT_PORT);
            bool end = false;
            std::string message = "";
            ProtobufMIST::Task parsed;
            rdo.establish();

            while (!end) {
                std::string x = rdo.receive_chunk(1);
                if ((x.find((char) 185) != std::string::npos) || x == "") {
                    end = true;
                } else {
                    message += x;
                }
            }

            if (parsed.ParseFromString(message)) {
                auto to_run = m_task_queue.find(parsed.task_name());
                if (to_run != m_task_queue.end())
                    run_task(to_run->second, MIST::TaskRunType::ASYNC);
            }
        }
    }

    void run_task(std::string id, MIST::TaskRunType run_as) {
        auto to_run = m_task_queue.find(id);
        if (to_run != m_task_queue.end())
            run_task(to_run->second, run_as);
    }

    void run_task(MIST::TaskInterface* task, MIST::TaskRunType run_as) {
        switch (run_as) {
        case MIST::TaskRunType::SYNC:
            task->run();
            break;
        case MIST::TaskRunType::ASYNC:
            m_async_cleanup.push_back(std::async(std::launch::async, &MIST::TaskInterface::run, task));
            break;
        }
    }

    int run_tasks(const std::vector<std::tuple<std::string, MIST::TaskRunType>> profile) {
        try {
            for (const auto& taskinfo : profile)
                run_task(std::get<0>(taskinfo), std::get<1>(taskinfo));
        } catch (std::exception e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    void send_task(std::string task, MIST::Machine machine, short int port) {
        SendData sd(machine.address, port);
        sd.establish();
        task += (char)182;
        sd.send_string(task);
    }

    void stop() {
        m_run_checker = false;
        for (auto& f : m_async_cleanup)
            f.get();
    }
};
