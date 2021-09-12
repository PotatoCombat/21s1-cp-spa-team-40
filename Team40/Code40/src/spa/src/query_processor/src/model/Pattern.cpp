#include "Pattern.h"

Pattern::Pattern(Reference stmt, Reference var, string pattern)
    : stmt(stmt), var(var), pattern(pattern) {}

Reference Pattern::getStmt() { return stmt; }

Reference Pattern::getVar() { return var; }

string Pattern::getPattern() { return pattern; }
