#pragma once

#include "Reference.h"
#include "ClauseType.h"

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
    Clause* Clause::copy(); // not working
    ~Clause();
};
