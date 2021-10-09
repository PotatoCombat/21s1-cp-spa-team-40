#pragma once

#include "ClauseType.h"
#include "Reference.h"

using namespace std;

class Clause {
private:
    ClauseType type;
    Reference first;
    Reference second;
    string pattern; // optional, for pattern clause only
    bool exactMatch; // optional, for pattern clause only

protected:
    Clause(ClauseType type, Reference first, Reference second, string pattern,
           bool exactMatch);
    const string DEFAULT_PATTERN = "";
    const bool DEFAULT_EXACT_MATCH = true;

public:
    Clause(ClauseType type, Reference first, Reference second);
    Reference *getFirstReference();
    Reference *getSecondReference();
    string getPattern();
    bool isExactMatch();
    ClauseType getType();
    bool equals(Clause &other);
    Clause* copy();
    ~Clause();
};
