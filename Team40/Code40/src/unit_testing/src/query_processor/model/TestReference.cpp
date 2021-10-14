#include "catch.hpp"

#include "query_processor/model/Reference.h"

struct TestReference {
    inline static const string VALUE = "a1";
    inline static const string OTHER_VALUE = "v";
    inline static const DesignEntityType ASSIGN_TYPE = DesignEntityType::ASSIGN;
    inline static const DesignEntityType VAR_TYPE = DesignEntityType::VARIABLE;
    inline static const ReferenceType SYN_TYPE = ReferenceType::SYNONYM;
    inline static const ReferenceType CONST_TYPE = ReferenceType::CONSTANT;
    inline static const ReferenceAttribute INT_TYPE =
        ReferenceAttribute::INTEGER;
    inline static const ReferenceAttribute NAME_TYPE = ReferenceAttribute::NAME;
    static Reference createReference();
    static Reference createOtherReference();
};

Reference TestReference::createReference() {
    return Reference(ASSIGN_TYPE, SYN_TYPE, VALUE, INT_TYPE);
}

Reference TestReference::createOtherReference() {
    return Reference(VAR_TYPE, CONST_TYPE, OTHER_VALUE, NAME_TYPE);
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
                      TestReference::VALUE, TestReference::INT_TYPE);
        REQUIRE(!ref.equals(refDiffRefType));
    }
}

TEST_CASE("Reference: copy") {
    Reference *ref =
        new Reference(TestReference::ASSIGN_TYPE, TestReference::SYN_TYPE,
                      TestReference::VALUE, TestReference::INT_TYPE);
    Reference *refCopy = ref->copy();

    REQUIRE(ref->equals(*refCopy));

    delete ref;
    delete refCopy;
}
