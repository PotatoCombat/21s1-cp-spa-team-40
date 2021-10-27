#include "common/model/Procedure.h"

#include <utility>

using namespace std;

Procedure::Procedure(ProcName name) : Entity<ProcName>(move(name)) { }

void Procedure::addToStmtLst(Statement *stmt) { this->stmtLst.push_back(stmt); }

vector<Statement *> Procedure::getStmtLst() { return this->stmtLst; }

bool Procedure::operator<(const Procedure &other) const {
    return name < other.name;
}

bool Procedure::operator==(const Procedure &other) const {
    if (name != other.name) {
        return false;
    }

    auto iter = stmtLst.begin();
    auto otherIter = other.stmtLst.begin();

    while (iter != stmtLst.end() && otherIter != stmtLst.end()) {
        if (*iter != *otherIter) {
            return false;
        }
        iter++;
        otherIter++;
    }

    return iter == stmtLst.end() && otherIter == stmtLst.end();
}
