#include "catch.hpp"

#include "Reference.h"

using namespace std;


TEST_CASE("Reference: equals") { 
    Reference ref1(DesignEntityType::PROCEDURE, ReferenceType::CONSTANT, "x");
    Reference ref2(DesignEntityType::PROCEDURE, ReferenceType::CONSTANT, "x");
    Reference ref3(DesignEntityType::PROCEDURE, ReferenceType::CONSTANT, "x2");
    Reference ref4(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM, "x");
    Reference ref5(DesignEntityType::STMT, ReferenceType::CONSTANT, "x");

    // all match
    REQUIRE(ref1.equals(ref2));
    // different value
    REQUIRE(!ref1.equals(ref3));
    // different reference type
    REQUIRE(!ref1.equals(ref4));
    // different design entity type
    REQUIRE(!ref1.equals(ref5));
}
