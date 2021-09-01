#include "catch.hpp"
#include "common/model/Variable.h"

TEST_CASE("Variable: Creates and retrieves value correctly") {
    const int VALUE = 10;
    const string NAME = "BLAH";
    Variable variable(VALUE, NAME);
    REQUIRE(variable.getValue() == VALUE);
    REQUIRE(variable.getName() == NAME);
}