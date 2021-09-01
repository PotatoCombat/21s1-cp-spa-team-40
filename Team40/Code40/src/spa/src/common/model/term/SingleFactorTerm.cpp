#include "../Factor.h"
#include "Term.h"
#include <iostream>

class SingleFactorTerm : public Term {
private:
    Factor factor;
    int value;

public:
    SingleFactorTerm(Factor factor)
        : value(NULL), factor(factor), Term(TermType::SINGLE_FACTOR){};

    int getValue() override {
        std::cout << "CALLED" << std::endl;
        if (value == NULL) {
            value = factor.getValue();
        }
        return value;
    }
};