#include "common/model/term/SingleFactorTerm.h"

SingleFactorTerm::SingleFactorTerm(Factor *factor)
    : Term(TermType::SINGLE_FACTOR, factor){};
