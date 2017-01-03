#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <functional>
#include "asio.hpp"

typedef void (*MIST_taskfunc)(void);

namespace MIST {
    namespace Internal {
        const char delimiter = 182;
    }
}
