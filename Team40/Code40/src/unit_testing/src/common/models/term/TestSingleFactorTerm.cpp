#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "common/model/term/SingleFactorTerm.h"

TEST_CASE("SingleFactorTerm: Creates and retrieves value correctly") {
    const string VALUE = "12";
    ConstantValue cv(VALUE);
    SingleFactorTerm s(&cv);
    REQUIRE(s.getFactor() == &cv);
}
