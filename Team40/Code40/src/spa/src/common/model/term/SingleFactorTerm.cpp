#include "../Factor.h"
#include "Term.h"
#include <iostream>

class SingleFactorTerm : public Term {
private:
    Factor factor;
    int value;

public:
    SingleFactorTerm(Factor factor)
        // Note that value is initialized to a dummy value here
        : value(0), factor(factor), Term(TermType::SINGLE_FACTOR){};

    int getValue() override {
        std::cout << "CALLED" << std::endl;
        if (!hasComputedValue) {
            value = factor.getValue();
            hasComputedValue = true;
        }
        return value;
    }
};