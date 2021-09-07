#pragma once

#include "Term.h"
#include "Factor.h"

class ModuloByFactorTerm : public Term {
private:
    Factor factor;
    Term *term;

public:
    ModuloByFactorTerm(Term *term, Factor factor);
    int getValue() override;
};
