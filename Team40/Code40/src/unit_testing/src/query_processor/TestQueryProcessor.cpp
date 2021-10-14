#include "catch.hpp"

#include <string>

#include "query_processor/QueryProcessor.h"

TEST_CASE("QueryProcessor: Select BOOLEAN = TRUE") {
    QueryProcessor qp(nullptr);
    list<string> results;

    string SELECT_BOOLEAN = "Select BOOLEAN";
    qp.processQuery(SELECT_BOOLEAN, results);

    REQUIRE(results.size() == 1);
    REQUIRE(results.front() == "TRUE");

    results.clear();
    SELECT_BOOLEAN = "stmt s; Select BOOLEAN";
    qp.processQuery(SELECT_BOOLEAN, results);

    REQUIRE(results.size() == 1);
    REQUIRE(results.front() == "TRUE");

    results.clear();
    SELECT_BOOLEAN = "Select BOOLEAN with 10 = 10";
    qp.processQuery(SELECT_BOOLEAN, results);

    REQUIRE(results.size() == 1);
    REQUIRE(results.front() == "TRUE");
}

TEST_CASE("QueryProcessor: Select BOOLEAN = FALSE") {
    QueryProcessor qp(nullptr);
    list<string> results;

    // invalid attribute
    string SELECT_BOOLEAN = "assign a; Select BOOLEAN with a.varName = 10";
    qp.processQuery(SELECT_BOOLEAN, results);

    REQUIRE(results.size() == 1);
    REQUIRE(results.front() == "FALSE");

    results.clear();
    // invalid type
    SELECT_BOOLEAN = "assign a; Select BOOLEAN with \"variable\" = 10";
    qp.processQuery(SELECT_BOOLEAN, results);

    REQUIRE(results.size() == 1);
    REQUIRE(results.front() == "FALSE");
}
