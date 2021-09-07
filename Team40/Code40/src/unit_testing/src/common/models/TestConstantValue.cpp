#include "common/model/ConstantValue.h"
#include "catch.hpp"

TEST_CASE("ConstantValue: Creates and retrieves value correctly") {
    const int VALUE = 10;
    ConstantValue c(VALUE);
    REQUIRE(c.getValue() == VALUE);
}
