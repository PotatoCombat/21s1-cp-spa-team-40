#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "common/model/term/ModuloByFactorTerm.h"
#include "common/model/term/SingleFactorTerm.h"

TEST_CASE("ModuloByFactorTerm: Creates and retrieves value correctly") {
    const int VALUE_1 = 12;
    const int VALUE_2 = 5;
    ConstantValue cv(VALUE_1);
    SingleFactorTerm s(&cv);
    ConstantValue c(VALUE_2);
    ModuloByFactorTerm m = ModuloByFactorTerm(&s, &c);
    REQUIRE(m.getValue() == VALUE_1 % VALUE_2);
}
