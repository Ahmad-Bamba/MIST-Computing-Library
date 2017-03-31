#pragma once

#include "SharedVariableBase.hpp"
#include <networking/ReceiveData.hpp>
#include <networking/SendData.hpp>

#include <string>

namespace MIST {
    class SharedString : public SharedVariableBase<std::string> {
    private:
        std::string* _raw;
    public:
        virtual ~SharedString() { delete _raw; }

        SharedString(std::string val) { _raw = &val; }

        virtual std::string getValue() { return *_raw; }
        virtual void copy(std::string &val) { val = *_raw; }
        virtual void updateLocal(std::string &val) { _raw = &val; }

        virtual void update(std::string &val) {
            /*Call updateLocal then broadcast the update signal to all computers on network*/
        }
    };
}
