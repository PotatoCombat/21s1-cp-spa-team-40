#include "catch.hpp"
#include "ConstantValue.h"
#include "SingleFactorTerm.cpp"

TEST_CASE("SingleFactorTerm: Creates and retrieves value correctly") {
    const int VALUE = 12;
    SingleFactorTerm s(ConstantValue(12));
    REQUIRE(s.getValue() == VALUE);
}
