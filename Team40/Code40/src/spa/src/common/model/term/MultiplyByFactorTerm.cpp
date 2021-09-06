#include "../Factor.h"
#include "Term.h"

class MultiplyByFactorTerm : public Term {
private:
    Factor factor;
    Term *term;

public:
    MultiplyByFactorTerm(Term *term, Factor factor)
        // Note that value is initialized to a dummy value here
        : factor(factor), term(term), Term(TermType::MULTIPLY_TERM_BY_FACTOR){};

    int getValue() override {
        if (!hasComputedValue) {
            value = term->getValue() * factor.getValue();
            hasComputedValue = true;
        }
        return value;
    }
};
