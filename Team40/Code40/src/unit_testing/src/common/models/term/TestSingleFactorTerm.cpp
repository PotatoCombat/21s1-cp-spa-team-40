#include "common/model/term/SingleFactorTerm.cpp"
#include "catch.hpp"
#include "common/model/ConstantValue.h"

TEST_CASE("SingleFactorTerm: Creates and retrieves value correctly") {
    const int VALUE = 12;
    SingleFactorTerm s(ConstantValue(12));
    REQUIRE(s.getValue() == VALUE);
}
