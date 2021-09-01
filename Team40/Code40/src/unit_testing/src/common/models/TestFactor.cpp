#include "common/model/Factor.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Factor: Creates and retrieves value correctly") {
    const int VALUE = 10;
    Factor factor(FactorType::EXPRESSION, VALUE);
    REQUIRE(factor.getValue() == VALUE);
}
