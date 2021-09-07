#pragma once

#include "Term.h"
#include "Factor.h"

class SingleFactorTerm : public Term {
private:
    Factor factor;

public:
    explicit SingleFactorTerm(Factor factor);
    int getValue() override;
};
