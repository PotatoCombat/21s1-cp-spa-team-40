#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "common/model/term/MultiplyByFactorTerm.cpp"
#include "common/model/term/SingleFactorTerm.cpp"

TEST_CASE("MultiplyByFactorTerm: Creates and retrieves value correctly") {
    const int VALUE_1 = 7;
    const int VALUE_2 = 8;
    SingleFactorTerm s = SingleFactorTerm(ConstantValue(VALUE_1));
    auto c = ConstantValue(VALUE_2);
    MultiplyByFactorTerm m = MultiplyByFactorTerm(&s, c);
    REQUIRE(m.getValue() == VALUE_1 * VALUE_2);
}
