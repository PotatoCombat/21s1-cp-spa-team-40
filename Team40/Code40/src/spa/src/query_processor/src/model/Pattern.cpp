#include "query_processor/model/Pattern.h"

Pattern::Pattern(Reference stmt, Reference var, string pattern)
    : stmt(stmt), var(var), pattern(pattern) {}

Reference *Pattern::getStmt() { return &this->stmt; }

Reference *Pattern::getVar() { return &this->var; }

string Pattern::getPattern() { return pattern; }
