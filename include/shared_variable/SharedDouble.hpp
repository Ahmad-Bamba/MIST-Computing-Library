#pragma once

/*

#include "SharedVariableBase.hpp"
#include <networking/Receive.hpp>
#include <networking/Send.hpp>

namespace MIST {
    class SharedDouble : public SharedVariableBase<double> {
    private:
        double* _raw;
    public:
        virtual ~SharedDouble() { delete _raw; }

        SharedDouble(double val) { _raw = &val; }

        virtual double getValue() { return *_raw; }
        virtual void copy(double &val) { val = *_raw; }
        virtual void updateLocal(double &val) { _raw = &val; }

        virtual void update(double &val) {
            /*Call updateLocal then broadcast the update signal to all computers on network
        }
    };
}
*/
