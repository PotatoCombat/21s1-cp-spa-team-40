#pragma once

#include "EntityTable.h"
#include "common/model/Variable.h"

using namespace std;

class VarTable : public EntityTable<Variable, VarIndex> {
public:
    VarTable();

    /// Stores the pointer \param variable in the table, and indexes the variable.
    /// \return index of the \param variable in the table.
    VarIndex insert(Variable *variable) override;

    /// Returns an iterator for all the variable names stored in the table.
    Iterator<VarName> getNames();

private:
    vector<VarName> names;
};
