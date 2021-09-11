#include "common/model/ConstantValue.h"

using namespace std;

ConstantValue::ConstantValue(int value)
    : Factor(FactorType::CONSTANT), value(value) {
    this->name = to_string(value);
}

int ConstantValue::getValue() { return value; }

string ConstantValue::getName() { return name; }
