#include "model/Procedure.h"

Procedure::Procedure(string name) : name(name) { this->stmtLst = {}; }

void Procedure::addToStmtLst(Statement stmt) { this->stmtLst.push_back(stmt); }

string Procedure::getName() { return this->name; };

vector<Statement> Procedure::getStmtLst() { return this->stmtLst; };
