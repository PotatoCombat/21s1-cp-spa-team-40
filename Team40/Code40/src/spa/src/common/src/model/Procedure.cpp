#include "common/model/Procedure.h"

Procedure::Procedure(string name) : name(name), stmtLst({}) {}

void Procedure::addToStmtLst(Statement *stmt) { this->stmtLst.push_back(stmt); }

string Procedure::getName() { return this->name; };

vector<Statement *> Procedure::getStmtLst() { return this->stmtLst; };

bool Procedure::operator==(const Procedure &other) const {
    bool sameName = name == other.name;
    bool sameStmtLst = stmtLst == other.stmtLst;
    return sameName && sameStmtLst;
}
