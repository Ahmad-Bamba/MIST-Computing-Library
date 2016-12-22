#pragma once
#include "MIST_Internal.hpp"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <asio.hpp>
#include <chrono>
#include <vector>
#include <atomic>
#include <string>

class SendData
    //HOW TO USE: Create SendData object
    //Any time you want to send data use SendData.send([data],[IP]);
    //Each call to Send will create a new detached thread on which data will be sent (so large data transfers don't cause stutters)
{
private:

    asio::io_service io;

    asio::ip::tcp::resolver resolver;

    //asio::ip::tcp::resolver::query query;

    //asio::ip::tcp::socket socket;

    std::atomic<bool> clean_up_needed;
    struct Job
    {
        bool isComplete;
        std::thread* job;
    };

    std::vector<Job> SendJobs; //true = finished, false = still sending
    std::atomic<unsigned long> number_of_send_jobs; //keeps track of numbers of long

    inline void send_string(std::string dataToSend, std::string IP, unsigned long id) {
        asio::ip::tcp::resolver::query query(IP.c_str(), std::to_string(id));
        asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        asio::ip::tcp::socket socket(io);
        asio::connect(socket, endpoint_iterator);
        asio::error_code ignored_error;
        asio::write(socket, asio::buffer(dataToSend.c_str(), dataToSend.length()), ignored_error);
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
                    SendJobs.erase(SendJobs.begin()+iterator); //if Thread is complete, delete it's object
                    number_of_send_jobs--;
                }
            } else if(iterator < number_of_send_jobs){
                if (SendJobs.at(iterator).isComplete)
                {
                    delete SendJobs[iterator].job; //delete thread and associated memory
                    SendJobs.erase(SendJobs.begin()+iterator); //if Thread is complete, delete it's object
                    number_of_send_jobs--;
                }
                iterator++;
            }   
        }
    }; //cleans up/deletes threads once they're done with their jobs

public:
    SendData(unsigned port)
        : resolver(io)
    {
       // clean_up_needed = true;
       // std::thread thread_cleanup_loop(&SendData::thread_cleanup_loop, this);
        //thread_cleanup_loop.detach();
    }; //creates at minimum one thread (for cleanup_loop)
    ~SendData() {
        
    };

    inline void Send(std::string data, std::string IP){
        size_t size;
        size = data.size();
        data.insert(data.begin(), size);
        std::thread * t = new std::thread(&SendData::send_string, this, data, IP, number_of_send_jobs+1);
        Job j;
        j.isComplete = false;
        j.job = t;
        SendJobs.push_back(j);
        t->detach();
        number_of_send_jobs++;
    }; //creates new instance of send_[data type] on detached thread
    inline void stop()
    {
        clean_up_needed = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //jank, but it lets us exit safely (allows thread_cleanup_loop to exit), will have better solution when science fair isn't a pressing concern.
    };
};