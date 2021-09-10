#include "pkb/VarTable.h"

using namespace std;

VarTable::VarTable() = default;

VarIndex VarTable::insert(Variable *variable) {
    names.push_back(variable->getName());
    return EntityTable<Variable, VarIndex>::insert(variable);
}

Iterator<VarName> VarTable::getNames() {
    return {names};
}
