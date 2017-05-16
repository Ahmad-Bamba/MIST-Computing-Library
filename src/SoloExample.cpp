#include <LibMIST.h>
#include <tuple>
#include <iostream>
#include <string>
#include <cmath>
#include <functional>

// Define an algorithm that runs tasks with the provided IDs
class DistributedTask : public MIST::Algorithm {
public:
    DistributedTask() : MIST::Algorithm() {
        add_sequential("print hi");
        add_parallel("square root of 4");
        add_sequential("1 + 1");
    }
};

// Define all the task functions

void printhi(std::tuple<noargs_mist> args) {
    std::cout << "Hi!" << std::endl;
}

void add(std::tuple<int, int> args) {
    int a = std::get<0>(args);
    int b = std::get<1>(args);
    std::cout << a << " + " << b << " = " << a + b << std::endl;
}

void root(std::tuple<float> args) {
    float i = std::get<0>(args);
    std::cout << "The square root of " << i << " is " << std::sqrt(i) << std::endl;
}

// this example runs a user-defined modular algorithm on a single PC
int main() {
    // create object
    DistributedTask distributed_task = DistributedTask();

    // create an object that represents this computer
    MIST::Machine this_pc("This PC");

    // set MIST computing to run all tasks on the local machine
    MIST::MIST computing(&this_pc, MIST::AlgorithmMode::SOLO, &distributed_task);

    // provide definitions for all tasks
    // they may need to be casted to std::function objects
    computing.add_task(std::string("print hi"), (std::function<void(std::tuple<noargs_mist>)>) printhi, { nullptr });
    computing.add_task(std::string("1 + 1"), (std::function<void(std::tuple<int, int>)>) add, { 1, 1 });
    computing.add_task(std::string("square root of 4"), (std::function<void(std::tuple<float>)>) root, { 4.0f });

    //run and return result
    return computing.run();
}
