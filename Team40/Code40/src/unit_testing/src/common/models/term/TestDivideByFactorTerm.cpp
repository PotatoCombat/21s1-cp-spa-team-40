#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "common/model/term/DivideByFactorTerm.h"
#include "common/model/term/SingleFactorTerm.h"

TEST_CASE("DivideByFactorTerm: Creates and retrieves value correctly") {
    const string VALUE_1 = "15";
    const string VALUE_2 = "5";
    ConstantValue cv(VALUE_1);
    SingleFactorTerm s = SingleFactorTerm(&cv);
    auto c = ConstantValue(VALUE_2);
    DivideByFactorTerm m = DivideByFactorTerm(&s, &c);
    REQUIRE(m.getTerm() == &s);
    REQUIRE(m.getFactor() == &c);
}
