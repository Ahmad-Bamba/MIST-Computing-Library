#pragma once

namespace MIST {
    template<typename T>
    class SharedVariableBase {
    public:
        SharedVariableBase() = default;
        virtual ~SharedVariableBase() = 0;

        virtual T getValue() = 0;
        virtual void copy(T &to) = 0;
        virtual void updateLocal(T &val) = 0;
        virtual void update(T &val) = 0;
    };
}
