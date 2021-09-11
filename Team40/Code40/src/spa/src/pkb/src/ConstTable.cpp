#include "pkb/ConstTable.h"

using namespace std;

ConstTable::ConstTable() = default;

void ConstTable::insert(ConstantValue *constant) {
    values.push_back(constant->getValue());
    EntityTable<ConstantValue, ConstName>::insert(constant)
}

Iterator<int> ConstTable::getValues() {
    return {values};
}
