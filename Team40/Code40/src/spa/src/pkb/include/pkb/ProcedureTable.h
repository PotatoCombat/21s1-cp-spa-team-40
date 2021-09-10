#pragma once

#include "EntityTable.h"
#include "common/model/Procedure.h"

using namespace std;

class ProcedureTable : public EntityTable<Procedure, ProcName> {
public:
    ProcedureTable();

    /// Stores the pointer \param procedure in the table, and indexes the procedure.
    /// \return index of the \param procedure in the table.
    void insert(Procedure *procedure) override;

};
