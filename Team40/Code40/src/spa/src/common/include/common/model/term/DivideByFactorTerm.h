#pragma once

#include "Term.h"
#include "model/Factor.h"

class DivideByFactorTerm : public Term {
private:
    Factor factor;
    Term *term;

public:
    DivideByFactorTerm(Term *term, Factor factor);
    int getValue() override;
};
