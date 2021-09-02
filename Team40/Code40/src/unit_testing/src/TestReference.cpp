#include "catch.hpp"

#include "query_processor/query_preprocessor/pql_model/EntityReference.h"
#include "query_processor/query_preprocessor/pql_model/Reference.h"
#include "query_processor/query_preprocessor/pql_model/StatementReference.h"

using namespace std;

EntityReference entRef1("SYNONYM");
EntityReference entRef2("NAMED_SYNONYM");
StatementReference stmtRef1("_");
StatementReference stmtRef2("1235134");

TEST_CASE("QP-EntityReference: constructor") {
    REQUIRE(entRef1.getValue() == "SYNONYM");
    REQUIRE(entRef2.getValue() == "NAMED_SYNONYM");
}

TEST_CASE("QP-StatementReference: constructor") {
    REQUIRE(stmtRef1.getValue() == "_");
    REQUIRE(stmtRef2.getValue() == "1235134");
}
