#pragma once

#include <string>
#include <vector>

#include "ClauseType.h"
#include "Reference.h"

using namespace std;

class Clause {
private:
    ClauseType type;
    Reference first;
    Reference second;
    vector<string> pattern; // empty for non-pattern clause
    bool exactMatch;        // true for non-pattern clause

    static const vector<string> DEFAULT_PATTERN;
    static const bool DEFAULT_EXACT_MATCH;

    Clause(ClauseType type, Reference first, Reference second,
           vector<string> pattern, bool exactMatch);

public:
    Clause(ClauseType type, Reference first, Reference second);
    Clause(Reference first, Reference second, vector<string> pattern,
           bool exactMatch);

    ClauseType getType();
    Reference *getFirstReference();
    Reference *getSecondReference();
    vector<string> getPattern();
    bool isExactMatch();
    bool equals(Clause &other);
    Clause *copy();
    ~Clause();
};
