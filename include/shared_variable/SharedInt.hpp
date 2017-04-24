#pragma once

/*

#include "SharedVariableBase.hpp"
#include <networking/Receive.hpp>
#include <networking/Send.hpp>

namespace MIST {
    class SharedInt : public SharedVariableBase<int> {
    private:
        int* _raw;
    public:
        virtual ~SharedInt() { delete _raw; }

        SharedInt(int val) { _raw = &val; }

        virtual int getValue() { return *_raw; }
        virtual void copy(int &val) { val = *_raw; }
        virtual void updateLocal(int &val) { _raw = &val; }

        virtual void update(int &val) {
            /*Call updateLocal then broadcast the update signal to all computers on network
        }
    };
}
*/
