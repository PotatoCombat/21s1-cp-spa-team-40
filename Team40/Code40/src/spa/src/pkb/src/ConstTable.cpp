#include "pkb/ConstTable.h"

using namespace std;

ConstTable::ConstTable() = default;

void ConstTable::insert(ConstantValue *constant) {
    values.push_back(constant->getValue());
}

Iterator<int> ConstTable::getValues() {
    return {values};
}
