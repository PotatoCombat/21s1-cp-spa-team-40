#pragma once

#include "Term.h"
#include "common/model/Factor.h"

class ModuloByFactorTerm : public Term {
private:
    Term *term;

public:
    ModuloByFactorTerm(Term *term, Factor *factor);
    int getValue() override;
};
