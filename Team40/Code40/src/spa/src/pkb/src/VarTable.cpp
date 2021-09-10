#include "pkb/VarTable.h"

using namespace std;

VarTable::VarTable() = default;

VarIndex VarTable::insert(Variable *variable) {
    varMap[variable->getName()] = variable;
    names.push_back(variable->getName());
    return EntityTable<Variable, VarIndex>::insert(variable);
}

Iterator<VarName> VarTable::getNames() {
    return {names};
}

Variable* VarTable::getVarByName(VarName var) {
    return varMap[var];
}
