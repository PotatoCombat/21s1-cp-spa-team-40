#pragma once

#include "Factor.h"
#include "string"

class ConstantValue : public Factor {
public:
    explicit ConstantValue(int value);
    std::string getName();
};
