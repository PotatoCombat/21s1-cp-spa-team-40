#include "Procedure.h"
#include <vector>

Procedure::Procedure(string name, vector<Statement> stmtLst)
    : name(name), stmtLst(stmtLst) {}

string Procedure::getName() { return this->name; };

vector<Statement> Procedure::getStmtLst() { return this->stmtLst; };
