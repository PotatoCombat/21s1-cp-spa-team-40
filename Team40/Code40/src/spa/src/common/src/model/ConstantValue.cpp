#include "common/model/ConstantValue.h"
#include <utility>
using namespace std;

ConstantValue::ConstantValue(string name)
    : Factor(FactorType::CONSTANT) {
    this->name = move(name);
}

string ConstantValue::getName() { return name; }

bool ConstantValue::operator<(const ConstantValue &other) const {
    return name < other.name;
}

bool ConstantValue::operator==(const ConstantValue &other) const {
    return name == other.name;
}
