#include "common/model/ConstantValue.h"

#include <utility>

using namespace std;

ConstantValue::ConstantValue(ConstName name) : Entity<ConstName>(move(name)) { }

bool ConstantValue::operator<(const ConstantValue &other) const {
    return name < other.name;
}

bool ConstantValue::operator==(const ConstantValue &other) const {
    return name == other.name;
}
