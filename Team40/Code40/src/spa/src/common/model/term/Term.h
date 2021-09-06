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
    int value;
    bool hasComputedValue; // Used for lazy initialization of value
    Term(TermType termType);
    Term(TermType termType, int value);

public:
    Term(TermType termType);
    Term(TermType termType, int value);
    TermType getTermType();
    virtual int getValue() = 0;
};
