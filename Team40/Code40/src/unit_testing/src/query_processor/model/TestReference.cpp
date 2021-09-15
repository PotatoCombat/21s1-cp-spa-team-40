#include "catch.hpp"

#include "query_processor/model/Reference.h"

struct TestReference {
    static const string VALUE;
    static const string OTHER_VALUE;
    static const DesignEntityType ASSIGN_TYPE;
    static const DesignEntityType VAR_TYPE;
    static const ReferenceType SYN_TYPE;
    static const ReferenceType CONST_TYPE;
    static Reference createReference();
    static Reference createOtherReference();
};

const string TestReference::VALUE = "a1";
const string TestReference::OTHER_VALUE = "v";
const DesignEntityType TestReference::ASSIGN_TYPE = DesignEntityType::ASSIGN;
const DesignEntityType TestReference::VAR_TYPE = DesignEntityType::VARIABLE;
const ReferenceType TestReference::SYN_TYPE = ReferenceType::SYNONYM;
const ReferenceType TestReference::CONST_TYPE = ReferenceType::CONSTANT;

Reference TestReference::createReference() {
    return Reference(ASSIGN_TYPE, SYN_TYPE, VALUE);
}

Reference TestReference::createOtherReference() {
    return Reference(VAR_TYPE, CONST_TYPE, OTHER_VALUE);
}

TEST_CASE("Reference: get methods") {
    Reference ref = TestReference::createReference();

    SECTION("test correct attributes") {
        REQUIRE(ref.getValue() == TestReference::VALUE);
        REQUIRE(ref.getDeType() == TestReference::ASSIGN_TYPE);
        REQUIRE(ref.getRefType() == TestReference::SYN_TYPE);
    }

    SECTION("test incorrect attributes") {
        REQUIRE(ref.getValue() != TestReference::OTHER_VALUE);
        REQUIRE(ref.getDeType() != TestReference::VAR_TYPE);
        REQUIRE(ref.getRefType() != TestReference::CONST_TYPE);
    }
}

TEST_CASE("Reference: equals") {
    Reference ref = TestReference::createReference();

    SECTION("test equals") {
        Reference refSame = TestReference::createReference();
        REQUIRE(ref.equals(refSame));
    }

    SECTION("test not equals") {
        // semantically wrong but just for testing purposes :)
        Reference refDiffRefType =
            Reference(TestReference::ASSIGN_TYPE, TestReference::CONST_TYPE,
                      TestReference::VALUE);
        REQUIRE(!ref.equals(refDiffRefType));
    }
}

TEST_CASE("Reference: copy") {
    Reference *ref =
        new Reference(TestReference::ASSIGN_TYPE, TestReference::SYN_TYPE,
                      TestReference::VALUE);
    Reference *refCopy = ref->copy();

    REQUIRE(ref->equals(*refCopy));

    delete ref;
    delete refCopy;
}
