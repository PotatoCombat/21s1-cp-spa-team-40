#pragma once
#include "common/model/Factor.h"

enum class TermType {
    SINGLE_FACTOR,
    MULTIPLY_TERM_BY_FACTOR,
    DIVIDE_TERM_BY_FACTOR,
    MODULO_TERM_BY_FACTOR,
};

class Term {
private:
    TermType termType;
    Factor *factor;

protected:
    int value;
    bool hasComputedValue; // Used for lazy initialization of value

public:
    Term(TermType termType, Factor *factor);
    TermType getTermType();
    virtual Factor *getFactor();
    virtual int getValue() = 0;
};
