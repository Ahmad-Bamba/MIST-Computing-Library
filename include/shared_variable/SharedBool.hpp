#pragma once

#include "SharedVariableBase.hpp"
#include <networking/ReceiveData.hpp>
#include <networking/SendData.hpp>

namespace MIST {
    class SharedBool : public SharedVariableBase<bool> {
    private:
        bool* _raw;
    public:
        virtual ~SharedBool() { delete _raw; }

        SharedBool(bool val) { _raw = &val; }

        virtual bool getValue() { return *_raw; }
        virtual void copy(bool &val) { val = *_raw; }
        virtual void updateLocal(bool &val) { _raw = &val; }

        virtual void update(bool &val) {
            /*Call updateLocal then broadcast the update signal to all computers on network*/
        }
    };
}
