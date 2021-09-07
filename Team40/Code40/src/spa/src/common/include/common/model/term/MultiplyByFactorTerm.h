#pragma once

#include "Term.h"
#include "common/model/Factor.h"

class MultiplyByFactorTerm : public Term {
private:
    Factor factor;
    Term *term;

public:
    MultiplyByFactorTerm(Term *term, Factor factor);
    int getValue() override;
};
