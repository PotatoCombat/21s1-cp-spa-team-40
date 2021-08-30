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

public:
    Term(TermType termType);

    TermType getTermType();

    virtual int getValue() = 0;
};
