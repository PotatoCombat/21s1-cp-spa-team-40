#include "pkb/ProcedureTable.h"

using namespace std;

ProcedureTable::ProcedureTable() = default;

ProcIndex ProcedureTable::insert(Procedure *procedure) {
    names.push_back(procedure->getName());
    return EntityTable<Procedure, ProcIndex>::insert(procedure);
}

Iterator<ProcName> ProcedureTable::getNames() {
    return {names};
}
