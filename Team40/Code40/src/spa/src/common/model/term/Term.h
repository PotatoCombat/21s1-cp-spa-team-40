#pragma once

enum class TermType {
    SINGLE_FACTOR,
    MULTIPLY_TERM_BY_FACTOR,
    DIVIDE_TERM_BY_FACTOR,
    MODULO_TERM_BY_FACTOR,
};

class Term {
private:
    TermType termType;

protected:
    bool hasComputedValue; // Used for lazy initialization of value
    Term(TermType termType);

public:
    TermType getTermType();

    virtual int getValue() = 0;
};
