#pragma once

#include "EntityTable.h"
#include "common/model/Procedure.h"

using namespace std;

class ProcedureTable : public EntityTable<Procedure, ProcIndex> {
public:
    ProcedureTable();

    /// Stores the pointer \param procedure in the table, and indexes the procedure.
    /// \return index of the \param procedure in the table.
    ProcIndex insert(Procedure *procedure) override;

    /// Returns an iterator for all the procedure names stored in the table.
    Iterator<ProcName> getNames();

    //Returns procedure with given procedure name
    Procedure* getProcByName(ProcName proc);

private:
    vector<ProcName> names;
    map<ProcName, Procedure*> procMap;
};
