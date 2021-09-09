#pragma once

#include "Factor.h"

class ConstantValue : public Factor {
    int value;

public:
    explicit ConstantValue(int value);
};
