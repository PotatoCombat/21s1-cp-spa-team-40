#include "query_processor/model/PatternClause.h"

PatternClause::PatternClause(Reference stmt, Reference var, string pattern)
    : stmt(stmt), var(var), pattern(pattern) {}

Reference PatternClause::getStmt() { return stmt; }

Reference PatternClause::getVar() { return var; }

string PatternClause::getPattern() { return pattern; }
