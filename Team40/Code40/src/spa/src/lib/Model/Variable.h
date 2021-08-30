#pragma once

#include "Factor.h"

class Variable: Factor{
    private:
        const int value;
        const string name;
    public:
        Variable(int value, string name);

        // getter
        string getName();
        int getValue() override; // Declared in Factor
};
