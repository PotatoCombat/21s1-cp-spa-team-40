#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "common/model/term/ModuloByFactorTerm.h"
#include "common/model/term/SingleFactorTerm.h"

TEST_CASE("ModuloByFactorTerm: Creates and retrieves value correctly") {
    const string VALUE_1 = "12";
    const string VALUE_2 = "5";
    ConstantValue cv(VALUE_1);
    SingleFactorTerm s(&cv);
    ConstantValue c(VALUE_2);
    ModuloByFactorTerm m = ModuloByFactorTerm(&s, &c);
    REQUIRE(m.getTerm() == &s);
    REQUIRE(m.getFactor() == &c);
    REQUIRE(m.getTermType() == TermType::MODULO_TERM_BY_FACTOR);
}
