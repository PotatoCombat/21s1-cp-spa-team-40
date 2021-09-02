#include "catch.hpp"

#include "query_processor/query_preprocessor/pql_model/EntityReference.h"
#include "query_processor/query_preprocessor/pql_model/Reference.h"
#include "query_processor/query_preprocessor/pql_model/StatementReference.h"

EntityReference entRef1("SYNONYM");
EntityReference entRef2("NAMED_SYNONYM");
StatementReference stmtRef1("_");
StatementReference stmtRef2("1235134");

TEST_CASE("QP-EntityReference: constructor, getValue") {
    Reference entRef("SYNONYM");
    REQUIRE(entRef1.getValue() == entRef.getValue());
    REQUIRE(entRef2.getValue() != entRef.getValue());
}

TEST_CASE("QP-EntityReference: getType") {
    Reference *entRef;
    entRef = &entRef1;
    REQUIRE(entRef1.getType() == entRef->getType());
    REQUIRE(entRef->getType() == ReferenceType::ENT_REF);
}

TEST_CASE("QP-StatementReference: constructor, getValue") {
    Reference entRef("1235134");
    REQUIRE(stmtRef1.getValue() != entRef.getValue());
    REQUIRE(stmtRef2.getValue() == entRef.getValue());
}

TEST_CASE("QP-StatementReference: getType") {
    Reference *entRef;
    entRef = &stmtRef1;
    REQUIRE(stmtRef1.getType() == entRef->getType());
    REQUIRE(entRef->getType() == ReferenceType::STMT_REF);
}
