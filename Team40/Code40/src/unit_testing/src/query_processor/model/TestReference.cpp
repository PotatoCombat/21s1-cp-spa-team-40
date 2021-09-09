#include "catch.hpp"

#include "Reference.h"

using namespace std;

struct TestReferenceHelper {
    Reference ref1;
    Reference ref2;
    Reference ref3;
    Reference ref4;
    Reference ref5;
};

TestReferenceHelper testReferenceHelper = {
    Reference(DesignEntityType::PROCEDURE, ReferenceType::CONSTANT, "x"),
    Reference(DesignEntityType::PROCEDURE, ReferenceType::CONSTANT, "x"),
    Reference(DesignEntityType::PROCEDURE, ReferenceType::CONSTANT, "x2"),
    Reference(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM, "x"),
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "x")
};

TEST_CASE("Reference: equals") { 
    // all match
    REQUIRE(testReferenceHelper.ref1.equals(testReferenceHelper.ref2));
    // different value
    REQUIRE(!testReferenceHelper.ref1.equals(testReferenceHelper.ref3));
    // different reference type
    REQUIRE(!testReferenceHelper.ref1.equals(testReferenceHelper.ref4));
    // different design entity type
    REQUIRE(!testReferenceHelper.ref1.equals(testReferenceHelper.ref5));
}
