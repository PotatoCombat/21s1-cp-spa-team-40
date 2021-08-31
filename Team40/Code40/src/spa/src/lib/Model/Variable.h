#pragma once

#include "Factor.h"
#include <string>

using namespace std;

class Variable: Factor {
    private:
        const string name;
    public:
        Variable(int value, string name);

        // getter
        string getName();
};
