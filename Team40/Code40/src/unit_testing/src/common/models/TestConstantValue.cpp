#include "common/model/ConstantValue.h"
#include "catch.hpp"

TEST_CASE("ConstantValue: Creates and retrieves name correctly") {
    const string VALUE = "10";
    ConstantValue c(VALUE);
    REQUIRE(c.getId() == VALUE);
}
