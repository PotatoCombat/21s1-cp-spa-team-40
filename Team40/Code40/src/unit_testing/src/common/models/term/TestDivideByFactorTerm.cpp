#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "common/model/term/DivideByFactorTerm.cpp"
#include "common/model/term/SingleFactorTerm.cpp"

TEST_CASE("DivideByFactorTerm: Creates and retrieves value correctly") {
    const int VALUE_1 = 15;
    const int VALUE_2 = 5;
    SingleFactorTerm s = SingleFactorTerm(ConstantValue(VALUE_1));
    auto c = ConstantValue(VALUE_2);
    DivideByFactorTerm m = DivideByFactorTerm(&s, c);
    REQUIRE(m.getValue() == VALUE_1 / VALUE_2);
}
