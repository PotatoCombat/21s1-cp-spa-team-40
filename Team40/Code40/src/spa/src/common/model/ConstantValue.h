#pragma once

#include "Factor.h"

class ConstantValue : public Factor {
public:
    explicit ConstantValue(int value);
};
