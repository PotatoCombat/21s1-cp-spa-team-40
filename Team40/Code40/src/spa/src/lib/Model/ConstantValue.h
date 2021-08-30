#pragma once

#include "Factor.h"

class ConstantValue: Factor {
    private:
        const int value;
    public:
        explicit ConstantValue(int value);

        // getter
        int getValue() override; // Declared in Factor
};