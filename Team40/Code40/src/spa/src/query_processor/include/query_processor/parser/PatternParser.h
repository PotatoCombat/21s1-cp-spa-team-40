#pragma once

#include <string>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/model/PatternClause.h"
#include "query_processor/model/Reference.h"

class PatternParser {
public:
    PatternParser();

    void initReferences(vector<Reference *> &declList);
    void clear();
    PatternClause *parse(PatTuple patTuple);

private:
    vector<Reference *> declList;

    string ident;
    vector<string> args;
};
