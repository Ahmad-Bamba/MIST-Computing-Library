#pragma once

#include "SharedVariableBase.hpp"
#include <networking/Receive.hpp>
#include <networking/Send.hpp>
#include <MIST.hpp>

namespace MIST {
    class SharedBool : public SharedVariableBase<bool> {
    public:
        SharedBool(bool val, MIST* op) : SharedVariableBase<bool>(val, op) { };

        SharedBool& operator=(const bool& other) {
            *_raw = other;
            return *this;
        }

        SharedBool& operator=(SharedBool& other) {
            *_raw = other.get_value();
            return *this;
        }

        bool operator!() {
            *_raw = !(*_raw);
            return *_raw;
        }

        virtual void update_local(bool &val) override {
            _raw = &val;
        }

        virtual void update(bool &val) {
            /*Call updateLocal then broadcast the update signal to all computers on network*/
        }
    };
}
