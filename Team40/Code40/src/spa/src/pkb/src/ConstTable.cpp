#include "pkb/ConstTable.h"

using namespace std;

ConstTable::ConstTable() = default;

void ConstTable::insert(ConstantValue *constant) {
    EntityTable<ConstantValue, ConstName>::insert(constant);
    values.push_back(constant->getValue());
}

Iterator<int> ConstTable::getValues() {
    return {values};
}
