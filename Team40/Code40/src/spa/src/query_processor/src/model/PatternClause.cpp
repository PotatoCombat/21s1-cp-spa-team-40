#include "query_processor/model/PatternClause.h"

PatternClause::PatternClause(Reference stmt, Reference var, string pattern, bool exactMatch)
    : stmt(stmt), var(var), pattern(pattern), exactMatch(exactMatch) {}

PatternClause::PatternClause(Reference stmt, Reference var)
    : PatternClause(stmt, var, "", true) {}

Reference *PatternClause::getStmt() { return &this->stmt; }

Reference *PatternClause::getVar() { return &this->var; }

string PatternClause::getPattern() { return pattern; }

bool PatternClause::isExactMatch() { return this->exactMatch; }

bool PatternClause::equals(PatternClause& other) {
    return this->getStmt()->equals(*(other.getStmt())) &&
        this->getVar()->equals(*(other.getVar())) &&
        this->getPattern() == other.getPattern();
}

PatternClause* PatternClause::copy() {
    Reference* r1 = this->stmt.copy();
    Reference* r2 = this->var.copy();
    return new PatternClause(*r1, *r2, this->pattern, this->exactMatch);
}

PatternClause::~PatternClause() { }

