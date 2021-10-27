#include "common/model/Procedure.h"

#include <utility>

using namespace std;

Procedure::Procedure(ProcName name) : Entity<ProcName>(move(name)) { }

void Procedure::addToStmtLst(Statement *stmt) { this->stmtLst.push_back(stmt); }

vector<Statement *> Procedure::getStmtLst() { return this->stmtLst; }

bool Procedure::operator<(const Procedure &other) const {
    return id < other.id;
}

bool Procedure::operator==(const Procedure &other) const {
    if (id != other.id) {
        return false;
    }

    auto iter = stmtLst.begin();
    auto otherIter = other.stmtLst.begin();

    while (iter != stmtLst.end() && otherIter != other.stmtLst.end()) {
        if (*iter != *otherIter) {
            return false;
        }
        iter++;
        otherIter++;
    }

    return iter == stmtLst.end() && otherIter == other.stmtLst.end();
}
