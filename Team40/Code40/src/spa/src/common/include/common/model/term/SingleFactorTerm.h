#pragma once

#include "Term.h"
#include "common/model/Factor.h"

class SingleFactorTerm : public Term {
public:
    explicit SingleFactorTerm(Factor *factor);
};
