#pragma once

#include "Factor.h"
#include "string"

class ConstantValue : public Factor {
    int value;

public:
    explicit ConstantValue(int value);

    int getValue();
    string getName();
};
