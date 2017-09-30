#pragma once

#include "SharedVariableBase.hpp"
#include <networking/Receive.hpp>
#include <networking/Send.hpp>

#include <string>

namespace MIST {
    class SharedString : public SharedVariableBase<std::string> {
    public:
        SharedString(std::string val, MIST* op) : SharedVariableBase<std::string>(val, op) { };

        SharedString& operator=(const std::string& other) {
            *_raw = other;
            return *this;
        }

        SharedString& operator=(SharedString& other) {
            *_raw = other.get_value();
            return *this;
        }

        SharedString& operator+=(std::string& other) {
            *_raw += other;
            return *this;
        }

        SharedString& operator+=(const char& other) {
            _raw->push_back(other);
            return *this;
        }

        char operator[](size_t i) {
            return (*_raw)[i];
        }

        virtual void update_local(std::string &val) override {
            _raw = &val;
        }

        virtual void update(std::string &val) override {
            /*Call updateLocal then broadcast the update signal to all computers on network*/
        }
    };
}
