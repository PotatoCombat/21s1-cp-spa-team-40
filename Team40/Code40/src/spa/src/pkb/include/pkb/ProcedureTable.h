#pragma once

#include "EntityTable.h"
#include "common/model/Procedure.h"

using namespace std;

class ProcedureTable : public EntityTable<Procedure, ProcName> {
public:
    ProcedureTable();
};
