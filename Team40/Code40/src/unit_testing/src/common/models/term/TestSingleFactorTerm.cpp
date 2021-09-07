#include "model/ConstantValue.h"
#include "model/SingleFactorTerm.h"
#include "catch.hpp"

TEST_CASE("SingleFactorTerm: Creates and retrieves value correctly") {
    const int VALUE = 12;
    SingleFactorTerm s(ConstantValue(12));
    REQUIRE(s.getValue() == VALUE);
}
