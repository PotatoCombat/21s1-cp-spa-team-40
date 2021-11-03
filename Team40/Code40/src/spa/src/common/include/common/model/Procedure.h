#pragma once

#include <string>
#include <vector>

#include "Abstractions.h"
#include "Entity.h"
#include "Statement.h"

using namespace std;

class Procedure : public Entity<ProcName> {
public:
    explicit Procedure(ProcName name);

    void addToStmtLst(Statement *stmt);

    vector<Statement *> getStmtLst();

    bool operator<(const Procedure &other) const;
    bool operator==(const Procedure &other) const;

private:
    vector<Statement *> stmtLst;
};
