#include "catch.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include "query_processor/QueryProcessor.h"
#include "pkb/PKB.h"

using namespace std;

struct TestInt_QP: public PKB {
public:
    inline static PKB createPKBWithOneStatement() {
        Statement s(1, StatementType::ASSIGN);

        PKB pkb;
        pkb.insertStmt(&s);

        return pkb;
    }
};

TEST_CASE("Integration: QP with empty PKB") {
    PKB pkb;
    QueryProcessor queryProcessor(&pkb);

    list<string> results;
    queryProcessor.processQuery("stmt s; Select s", results);

    REQUIRE(results.empty());
}

TEST_CASE("Integration: QP with 1 statement in PKB") {
    PKB pkb = TestInt_QP::createPKBWithOneStatement();
    QueryProcessor queryProcessor(&pkb);

    list<string> results;
    queryProcessor.processQuery("stmt s; Select s", results);

    REQUIRE(results.size() == 1);
    REQUIRE(*results.begin() == "1");
}

TEST_CASE("Integration: QP with 1 assignment statement in PKB") {
    PKB pkb = TestInt_QP::createPKBWithOneStatement();
    QueryProcessor queryProcessor(&pkb);

    list<string> results;
    queryProcessor.processQuery("assign a; Select a", results);

    REQUIRE(results.size() == 1);
    REQUIRE(*results.begin() == "1");
}
