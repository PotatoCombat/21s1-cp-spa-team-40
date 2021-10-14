#include "catch.hpp"

#include <string>

#include "query_processor/QueryProcessor.h"

TEST_CASE("QueryProcessor: Select BOOLEAN = TRUE") {
    const string SELECT_BOOLEAN = "Select BOOLEAN";

    QueryProcessor qp(nullptr);
    list<string> results;

    qp.processQuery(SELECT_BOOLEAN, results);

    REQUIRE(results.size() == 1);
    REQUIRE(results.front() == "TRUE");
}

TEST_CASE("QueryProcessor: Select BOOLEAN = FALSE") {
    // invalid attribute
    const string SELECT_BOOLEAN =
        "assign a; Select BOOLEAN with a.procName = 10";

    QueryProcessor qp(nullptr);
    list<string> results;

    qp.processQuery(SELECT_BOOLEAN, results);

    REQUIRE(results.size() == 1);
    REQUIRE(results.front() == "FALSE");
}
