#pragma once
#include "MIST_Internal.hpp"
#include <vector>
#include <chrono>
#include <atomic>

class SendData
    //HOW TO USE: Create SendData object
    //Any time you want to send data use SendData.send([data],[IP]);
    //Each call to Send will create a new detached thread on which data will be sent (so large data transfers don't cause stutters)
{
private:
    bool clean_up_needed;
    struct Job
    {
        bool isComplete;
        std::thread* job;
    };

    std::vector<Job> SendJobs; //true = finished, false = still sending
    std::atomic<unsigned long> number_of_send_jobs; //keeps track of numbers of long

    inline void send_string(std::string dataToSend, std::string IP, unsigned long id) {
        std::cout << dataToSend << std::endl;
        std::cout << IP << std::endl;
        SendJobs[id].isComplete = true;
    }; //actually sends the string

    void thread_cleanup_loop() {
        int iterator = 0;
        
        while (clean_up_needed)
        {
            if (iterator >= number_of_send_jobs) {
                iterator = 0;
            } else if (iterator == number_of_send_jobs) {
                if (SendJobs.at(iterator).isComplete)
                {
                    delete SendJobs[iterator].job; //delete thread and associated memory
                    SendJobs.erase(SendJobs.begin() + iterator); //if Thread is complete, delete it's object
                    number_of_send_jobs--;
                }
            } else {
                if (SendJobs.at(iterator).isComplete)
                {
                    delete SendJobs[iterator].job; //delete thread and associated memory
                    SendJobs.erase(SendJobs.begin() + iterator); //if Thread is complete, delete it's object
                    number_of_send_jobs--;
                }
                iterator++;
            }   
        }
    }; //cleans up/deletes threads once they're done with their jobs

public:
    SendData() {
        clean_up_needed = true;
        std::thread thread_cleanup_loop(&SendData::thread_cleanup_loop, this);
        thread_cleanup_loop.detach();
    };
    ~SendData() {

    };

    inline void Send(std::string data, std::string IP){
        std::thread * t = new std::thread(&SendData::send_string, this, data, IP, number_of_send_jobs+1);
        Job j;
        j.isComplete = false;
        j.job = t;
        SendJobs.push_back(j);
        t->detach();
        number_of_send_jobs++;
    }; //creates new instance of send_[data type] on detached thread
};