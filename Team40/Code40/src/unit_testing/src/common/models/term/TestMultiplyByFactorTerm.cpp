#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "common/model/term/MultiplyByFactorTerm.h"
#include "common/model/term/SingleFactorTerm.h"

TEST_CASE("MultiplyByFactorTerm: Creates and retrieves value correctly") {
    const string VALUE_1 = "7";
    const string VALUE_2 = "8";
    ConstantValue cv = ConstantValue(VALUE_1);
    Factor *f = &cv;
    SingleFactorTerm s = SingleFactorTerm(f);
    auto c = ConstantValue(VALUE_2);
    MultiplyByFactorTerm m = MultiplyByFactorTerm(&s, &c);
    REQUIRE(m.getTerm() == &s);
    REQUIRE(m.getFactor() == &c);
    REQUIRE(m.getTermType() == TermType::MULTIPLY_TERM_BY_FACTOR);
}
