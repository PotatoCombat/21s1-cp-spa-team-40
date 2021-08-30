#include "ConstantValue.h"

ConstantValue::ConstantValue(int value): value(value), Factor(FactorType::CONSTANT) {}

int ConstantValue::getValue() {
    return this->value;
}