#include "common/model/ConstantValue.h"

using namespace std;

ConstantValue::ConstantValue(int value)
    : Factor(FactorType::CONSTANT), value(value) {}

string ConstantValue::getName() { return to_string(this->getValue()); }

int ConstantValue::getValue() { return value; }