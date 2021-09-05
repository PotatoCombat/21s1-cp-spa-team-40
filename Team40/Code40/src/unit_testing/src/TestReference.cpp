#include "catch.hpp"

#include "query_processor/query_preprocessor/pql_model/EntityReference.h"
#include "query_processor/query_preprocessor/pql_model/Reference.h"
#include "query_processor/query_preprocessor/pql_model/StatementReference.h"

struct TestReference {
    static const string VALUE;
    static const string OTHER_VALUE;
    static Reference createReference();
    static Reference createOtherReference();
    static EntityReference createEntityReference();
    static StatementReference createStatementReference();
};

const string TestReference::VALUE = "s1";
const string TestReference::OTHER_VALUE = "s2";

Reference TestReference::createReference() {
    return Reference(VALUE);
}

Reference TestReference::createOtherReference() {
    return Reference(OTHER_VALUE);
}

EntityReference TestReference::createEntityReference() {
    return EntityReference(VALUE);
}

StatementReference TestReference::createStatementReference() {
    return StatementReference(VALUE);
}

TEST_CASE("QP-Reference: ctor, getValue") {
    EntityReference entRef = TestReference::createEntityReference();
    StatementReference stmtRef = TestReference::createStatementReference();
    
    REQUIRE(entRef.getValue() == TestReference::VALUE);
    REQUIRE(stmtRef.getValue() == TestReference::VALUE);
}

TEST_CASE("QP-Reference: getType") {
    EntityReference entRef = TestReference::createEntityReference();
    StatementReference stmtRef = TestReference::createStatementReference();

    SECTION("Test not equals unknown type") {
        Reference ref = TestReference::createReference();
        Reference* ref1 = &entRef;
        Reference* ref2 = &stmtRef;
        REQUIRE(ref1->getType() != ref.getType());
        REQUIRE(ref2->getType() != ref.getType());
    }

    SECTION("Test equals type") {
        REQUIRE(entRef.getType() == ReferenceType::ENT_REF);
        REQUIRE(stmtRef.getType() == ReferenceType::STMT_REF);
    }
}

TEST_CASE("QP-Reference: operator==") {
    Reference ref1 = TestReference::createReference();

    SECTION("test equals") {
        Reference ref2 = TestReference::createReference();
        REQUIRE(ref1 == ref2);
    }
    
    SECTION("test not equals") {
        Reference ref2 = TestReference::createOtherReference();
        REQUIRE(!(ref1 == ref2));
    }
}
