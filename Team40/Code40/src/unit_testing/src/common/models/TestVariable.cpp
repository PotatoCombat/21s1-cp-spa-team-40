#include "catch.hpp"
#include "common/model/Variable.h"

TEST_CASE("Variable: Creates and retrieves value correctly") {
    const string NAME = "BLAH";
    Variable variable(NAME);
    REQUIRE(variable.getName() == NAME);
}
