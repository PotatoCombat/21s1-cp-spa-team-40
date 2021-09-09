#pragma once

#include "Term.h"
#include "common/model/Factor.h"

class DivideByFactorTerm : public Term {
private:
    Term *term;

public:
    DivideByFactorTerm(Term *term, Factor *factor);
    int getValue() override;
};
