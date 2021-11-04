#pragma once

#include <string>
#include <vector>

#include "common/model/Abstractions.h"
#include "common/model/Entity.h"
#include "common/model/Statement.h"

using namespace std;

class Procedure : public Entity<ProcName> {
public:
    Procedure();
    explicit Procedure(ProcName name);

    void addToStmtLst(Statement *stmt);

    vector<Statement *> getStmtLst();

    bool operator<(const Procedure &other) const;
    bool operator==(const Procedure &other) const;

private:
    vector<Statement *> stmtLst;
};
