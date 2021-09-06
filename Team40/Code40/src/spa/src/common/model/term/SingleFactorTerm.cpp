#include "../Factor.h"
#include "Term.h"

class SingleFactorTerm : public Term {
private:
    Factor factor;

public:
    SingleFactorTerm(Factor factor)
        : factor(factor), Term(TermType::SINGLE_FACTOR){};

    int getValue() override {
        if (!hasComputedValue) {
            value = factor.getValue();
            hasComputedValue = true;
        }
        return value;
    }
};
