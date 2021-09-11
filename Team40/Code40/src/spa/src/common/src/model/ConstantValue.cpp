#include "common/model/ConstantValue.h"

ConstantValue::ConstantValue(int value)
    : Factor(FactorType::CONSTANT), value(value) {}

int ConstantValue::getValue() { return value; }