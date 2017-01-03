#pragma once
//#include "MIST_Internal.hpp"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <asio.hpp>
#include <chrono>
#include <vector>
#include <atomic>
#include <string>

using asio::ip::tcp;

typedef unsigned short ushort;

namespace MIST {
    class SendData {
        //HOW TO USE: Create SendData object
        //Any time you want to send data use SendData.send([data],[IP]);
        //Each call to Send will create a new detached thread on which data will be
        //sent (so large data transfers don't cause stutters)
    private:
        asio::io_service io;
        asio::ip::tcp::resolver resolver;
        asio::ip::tcp::resolver::query query;
        asio::ip::tcp::socket socket;
        asio::ip::tcp::resolver::iterator endpoint_iterator;

        std::atomic<bool> clean_up_needed;

        std::string IP;
        ushort port;

        struct Job {
            bool isComplete;
            std::thread* job;
        };

        std::vector<Job> SendJobs; //true = finished, false = still sending
        std::atomic<unsigned long> number_of_send_jobs; //keeps track of numbers of long

        inline size_t send_string(std::string dataToSend, unsigned long id = 0) {
            asio::connect(this->socket, this->endpoint_iterator);
            //asio::error_code ignored_error;
            //SendJobs[id].isComplete = true;
            std::string MISTCompliant = "AAAABBBB"; //identifies the type of data, can be checked by user, is checked by MIST
            dataToSend += "^^^^^^^^";
            MISTCompliant += dataToSend;
            return asio::write(socket, asio::buffer(MISTCompliant.c_str(), MISTCompliant.length())); //identifier sent
            //return asio::write(socket, asio::buffer(dataToSend.c_str(), dataToSend.length()));
        }; //actually sends the string

        void thread_cleanup_loop() {
            int iterator = 0;

            while (clean_up_needed) {
                if (iterator >= number_of_send_jobs) {
                    iterator = 0;
                } else if (iterator == number_of_send_jobs) {
                    if (SendJobs.at(iterator).isComplete)
                    {
                        delete SendJobs[iterator].job; //delete thread and associated memory
                        SendJobs.erase(SendJobs.begin()+iterator); //if Thread is complete, delete it's object
                        number_of_send_jobs--;
                    }
                } else if(iterator < number_of_send_jobs) {
                    if (SendJobs.at(iterator).isComplete) {
                        delete SendJobs[iterator].job; //delete thread and associated memory
                        SendJobs.erase(SendJobs.begin()+iterator); //if Thread is complete, delete it's object
                        number_of_send_jobs--;
                    }
                    iterator++;
                }
            }
        }; //cleans up/deletes threads once they're done with their jobs

    public:
        SendData(std::string IP, ushort port) //TODO
            : resolver(io),
              query(IP, std::to_string(port)),
              socket(io) {
           this->IP = IP;
           this->port = port;
           this->endpoint_iterator = resolver.resolve(this->query);

        }; //creates at minimum one thread (for cleanup_loop)
        ~SendData() { };

        inline void Send(std::string data, std::string IP) {
            //Is this necessary?
            //size_t size;
            //size = data.size();
            //data.insert(data.begin(), size);
            std::thread * t = new std::thread(&SendData::send_string, this, data, number_of_send_jobs + 1);
            Job j;
            j.isComplete = false;
            j.job = t;
            SendJobs.push_back(j);
            t->detach();
            number_of_send_jobs++;
        }; //creates new instance of send_[data type] on detached thread

        inline void simple_send(std::string data) {
            //EXTREME ASIO WRAPPING!!!
            send_string(data);
        }

        inline void stop() {
            clean_up_needed = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); //jank, but it lets us exit safely (allows thread_cleanup_loop to exit), will have better solution when science fair isn't a pressing concern.
        };
    };
}
