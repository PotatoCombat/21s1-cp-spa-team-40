#include "Procedure.h"
#include <vector>

Procedure::Procedure(string name) : name(name) { this->stmtLst = {}; }

void Procedure::addStatement(Statement statement) {
    this->stmtLst.push_back(statement);
};

string Procedure::getName() { return this->name; };

vector<Statement> Procedure::getStmtLst() { return this->stmtLst; };
