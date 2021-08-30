#pragma once

#include "Factor.h"

class ConstantValue: Factor {
    private:
        const int value;
    public:
        explicit ConstantValue(int value);
        int getValue() override;
};