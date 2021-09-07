#include "Term.h"
#include "Factor.h"

class ModuloByFactorTerm : public Term {
private:
    Factor factor;
    Term *term;

public:
    ModuloByFactorTerm(Term *term, Factor factor)
        : factor(factor), term(term), Term(TermType::MODULO_TERM_BY_FACTOR){};

    int getValue() override {
        if (!hasComputedValue) {
            value = term->getValue() % factor.getValue();
            hasComputedValue = true;
        }
        return value;
    }
};
