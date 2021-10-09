#pragma once

#include <string>

#include "Reference.h"
#include "query_processor/model/Clause.h"

using namespace std;

//enum class PATTERN_MATCH {
//    EXACT_MATCH, SUB_MATCH
//};

class PatternClause : public Clause{
public:
    PatternClause(Reference stmt, Reference var, string pattern, bool isExactMatch);
    PatternClause(Reference stmt, Reference var);
};
