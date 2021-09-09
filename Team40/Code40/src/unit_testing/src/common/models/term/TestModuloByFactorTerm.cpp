#include "common/model/ConstantValue.h"
#include "common/model/term/ModuloByFactorTerm.h"
#include "common/model/term/SingleFactorTerm.h"
#include "catch.hpp"

TEST_CASE("ModuloByFactorTerm: Creates and retrieves value correctly") {
    const int VALUE_1 = 12;
    const int VALUE_2 = 5;
    SingleFactorTerm s = SingleFactorTerm(ConstantValue(VALUE_1));
    auto c = ConstantValue(VALUE_2);
    ModuloByFactorTerm m = ModuloByFactorTerm(&s, c);
    REQUIRE(m.getValue() == VALUE_1 % VALUE_2);
}
