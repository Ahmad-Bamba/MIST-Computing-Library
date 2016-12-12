#pragma once
#include <string>
#include <thread>
#include <chrono>
#include <vector>

class RecieveData
{
private:
    unsigned long number_of_recieve_jobs;

    struct RecieveJob
    {
        std::thread * job;
        bool isComplete;
    };
    bool clean_up_needed;
    std::vector<RecieveJob> RecieveJobs;

    auto listen()
    {
        std::string data;
        //
        return data;
    }

    void thread_clean_up_loop()
    {
        int iterator = 0;

        while (clean_up_needed)
        {
            if (iterator >= number_of_recieve_jobs) {
                iterator = 0;
            }
            else if (iterator == number_of_recieve_jobs) {
                if (RecieveJobs.at(iterator).isComplete)
                {
                    delete RecieveJobs[iterator].job; //delete thread and associated memory
                    RecieveJobs.erase(RecieveJobs.begin() + iterator); //if Thread is complete, delete it's object
                    number_of_recieve_jobs--;
                }
            }
            else {
                if (RecieveJobs.at(iterator).isComplete)
                {
                    delete RecieveJobs[iterator].job; //delete thread and associated memory
                    RecieveJobs.erase(RecieveJobs.begin() + iterator); //if Thread is complete, delete it's object
                    number_of_recieve_jobs--;
                }
                iterator++;
            }
        }
    }

public:
    RecieveData()
    {
        std::thread clean_up_thread(&RecieveData::thread_clean_up_loop, this);
        clean_up_thread.detach();
    };

    ~RecieveData()
    {

    };

    auto Listen(std::string port)
    {
        std::string data;
        return data;
    };

    void stop()
    {
        clean_up_needed = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    };
};