#pragma once

#include "Term.h"
#include "common/model/Factor.h"

class SingleFactorTerm : public Term {
private:
    Factor factor;

public:
    explicit SingleFactorTerm(Factor factor);
    int getValue() override;
};
