#pragma once

#include "EntityTable.h"
#include "common/model/Variable.h"

using namespace std;

class VarTable : public EntityTable<Variable, VarName> {
public:
    VarTable();
};
