#pragma once

#include <MIST.hpp>

namespace MIST {
    template<typename T>
    class SharedVariableBase {
    protected:
        T* _raw { };
        MIST* _current_op { };
    public:
        SharedVariableBase(T val, MIST* op) {
            *_raw = val;
            _current_op = op;
        }

        SharedVariableBase(const SharedVariableBase& from) {
            *_raw = from.get_value();
        }

        SharedVariableBase(SharedVariableBase&& move) {
            move.updateLocal(*_raw);
            delete _raw;
        }

        ~SharedVariableBase() {
            delete _raw;
        }

        virtual T* get_raw() {
            return _raw;
        }

        virtual T get_value() {
            return *_raw;
        }

        virtual void copy(T& to) {
            to = *_raw;
        }

        virtual void update_local(T& val) = 0;
        virtual void update(T& val) = 0;
    };
}
