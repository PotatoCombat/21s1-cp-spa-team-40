#include "common/model/ConstantValue.h"

ConstantValue::ConstantValue(int value) : Factor(FactorType::CONSTANT, value) {}
std::string ConstantValue::getName() { return std::to_string(this->getValue()); }
