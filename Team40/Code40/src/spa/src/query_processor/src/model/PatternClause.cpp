#include "query_processor/model/PatternClause.h"

PatternClause::PatternClause(Reference stmt, Reference var, string pattern)
    : stmt(stmt), var(var), pattern(pattern) {}

Reference *PatternClause::getStmt() { return &this->stmt; }

Reference *PatternClause::getVar() { return &this->var; }

string PatternClause::getPattern() { return pattern; }
