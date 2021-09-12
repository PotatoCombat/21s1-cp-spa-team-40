#pragma once

#include "ClauseType.h"
#include "Reference.h"

using namespace std;

class Clause {
private:
    ClauseType type;
    Reference first;
    Reference second;

public:
    Clause(ClauseType type, Reference first, Reference second);
    Reference *getFirstReference();
    Reference *getSecondReference();
    ClauseType getType();
    bool equals(Clause &other);
    Clause* copy(); // not working
    ~Clause();
};
