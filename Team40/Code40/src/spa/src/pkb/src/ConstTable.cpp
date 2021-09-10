#include "pkb/ConstTable.h"

using namespace std;

ConstTable::ConstTable() = default;

ConstIndex ConstTable::insert(ConstantValue *constant) {
    values.push_back(constant->getValue());
    return EntityTable<ConstantValue, ConstIndex>::insert(constant);
}

Iterator<int> ConstTable::getValues() {
    return {values};
}
