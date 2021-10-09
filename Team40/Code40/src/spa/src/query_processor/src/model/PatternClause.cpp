#include "query_processor/model/PatternClause.h"

PatternClause::PatternClause(Reference stmt, Reference var, string pattern, bool exactMatch)
    : Clause(ClauseType::PATTERN, stmt, var, pattern, exactMatch) {}

PatternClause::PatternClause(Reference stmt, Reference var)
    : PatternClause(stmt, var, Clause::DEFAULT_PATTERN, Clause::DEFAULT_EXACT_MATCH) {}
