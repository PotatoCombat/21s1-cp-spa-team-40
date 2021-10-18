#include "catch.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include "TestIntegrationUtils.h"
#include "query_processor/QueryProcessor.h"
#include "pkb/PKB.h"

using namespace std;

struct TestInt_QP: public PKB {
public:
    inline static PKB createPKBWithOneStatement() {
        auto stmt = new Statement(1, StatementType::ASSIGN);

        PKB pkb;
        pkb.insertStmt(stmt);

        delete stmt;
        return pkb;
    }

    inline static PKB createPKB() {
        auto stmt = new Statement(1, StatementType::ASSIGN);

        PKB pkb;
        pkb.insertStmt(stmt);

        delete stmt;
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

TEST_CASE("Integration: QP process Source1") {
    PKB pkb = TestIntegrationUtils::createExamplePKB();
    QueryProcessor queryProcessor(&pkb);

    // Evaluating such that relationships
    set<string> tests1 { "2", "3", "4", "7", "8", "14", };
    list<string> results1;

    queryProcessor.processQuery(R"(stmt s; Select s such that Follows*(1, s))", results1);
    REQUIRE(tests1.size() == results1.size());
    for (auto & it : results1){
        REQUIRE(tests1.find(it) != tests1.end());
    }

    // Evaluating patterns relationships
    set<string> tests2 { "2", };
    list<string> results2;

    queryProcessor.processQuery(R"(assign a; Select a pattern a("y", "1"))", results2);
    REQUIRE(tests2.size() == results2.size());
    for (auto & it : results2){
        REQUIRE(tests2.find(it) != tests2.end());
    }
}
