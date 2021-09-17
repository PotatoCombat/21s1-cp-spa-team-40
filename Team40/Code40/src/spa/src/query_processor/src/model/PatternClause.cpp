#include "query_processor/model/PatternClause.h"

PatternClause::PatternClause(Reference stmt, Reference var, string pattern)
    : stmt(stmt), var(var), pattern(pattern) {}

Reference *PatternClause::getStmt() { return &this->stmt; }

Reference *PatternClause::getVar() { return &this->var; }

string PatternClause::getPattern() { return pattern; }

bool PatternClause::equals(PatternClause& other) {
    return this->getStmt()->equals(*(other.getStmt())) &&
        this->getVar()->equals(*(other.getVar())) &&
        this->getPattern() == other.getPattern();
}

PatternClause* PatternClause::copy() {
    Reference* r1 = this->stmt.copy();
    Reference* r2 = this->var.copy();
    return new PatternClause(*r1, *r2, this->pattern);
}

PatternClause::~PatternClause() { }

