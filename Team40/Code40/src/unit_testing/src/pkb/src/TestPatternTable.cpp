#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "pkb/PatternTable.h"

#include "catch.hpp"

using namespace std;

struct TestPatterns {
    inline static vector<string> createExprList1() {
        // ((x + 10) * (y - z) / 5)
        return {
            "(", "(", "x", "+", "10", ")", "*", "(",
            "y", "-", "z", ")", "/",  "5", ")",
        };
    }

    inline static vector<string> createExprList2() {
        // v + x * y + z * t
        return {
            "v", "+", "x", "*", "y", "+", "z", "*", "t",
        };
    }

    inline static set<string> createPatterns1() {
        // ((x + 10) * (y - z) / 5)
        return {
            "x",
            "10",
            "(x+10)",
            "y",
            "z",
            "(y-z)",
            "((x+10)*(y-z))",
            "5",
            "(((x+10)*(y-z))/5)",
        };
    }
};

TEST_CASE("PatternTable: createPostfix -> createPatterns") {
    vector<string> exprList = TestPatterns::createExprList1();
    vector<string> postfix = PatternTable::createPostfix(exprList);

    set<string> testPatterns = TestPatterns::createPatterns1();
    set<string> actualPatterns = PatternTable::createPatterns(postfix);

    REQUIRE(testPatterns.size() == actualPatterns.size());

    for (auto &p : actualPatterns) {
        testPatterns.erase(p);
    }

    REQUIRE(testPatterns.empty());
}

TEST_CASE("PatternTable: insertPattern") {
    Variable v("x");
    Statement stmt(1, StatementType::ASSIGN);
    stmt.setVariable(&v);
    stmt.setExpressionLst(TestPatterns::createExprList2());

    PatternTable table;
    table.insertPatternAssign(&stmt);

    REQUIRE(table.assignMatchesPattern(1, v.getName(), "v+x*y"));
    REQUIRE(table.assignMatchesPattern(1, "_", "v+x*y"));
}

// TEST_CASE("PatternTable: ctor") {
//     PatternTable table;
//     REQUIRE(table.getSize() == 0);
// }
//
// TEST_CASE("PatternTable: insert/getEntity") {
//     EntityTable<TestEntity, string> table;
//     TestEntity test("hello");
//     table.insert(&test);
//
//     REQUIRE(table.getSize() == 1);
//     REQUIRE(table.getEntity("hello") == &test);
// }
//
// TEST_CASE("EntityTable: entity not stored in table") {
//     auto table = TestEntityTable::createTable();
//     REQUIRE(table.getEntity("goodbye") == nullptr);
// }
//
// TEST_CASE("EntityTable: getNames") {
//     auto table = TestEntityTable::createTable();
//
//     vector<TestEntity> test = TestEntityTable::createItems();
//     vector<string> actual = table.getNames().asVector();
//
//     for (int i = 0; i < actual.size(); i++) {
//         REQUIRE(test.at(i).getName() == actual.at(i));
//     }
// }
//
// TEST_CASE("EntityTable: getSize") {
//     auto table = TestEntityTable::createTable();
//     REQUIRE(table.getSize() == TestEntityTable::createItems().size());
// }
