#include "common/model/term/SingleFactorTerm.cpp"
#include "catch.hpp"
#include "common/model/Variable.h"

TEST_CASE("SingleFactorTerm: Creates and retrieves value correctly") {
    const int VALUE = 12;
    SingleFactorTerm s(Variable(12, "BLAH"));
    REQUIRE(s.getValue() == VALUE);
}