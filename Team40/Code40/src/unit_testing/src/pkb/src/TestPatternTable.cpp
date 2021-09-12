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
        return vector<string>{"(", "(", "x", "+", "10", ")", "*", "(", "y", "-", "z", ")", "/",  "5", ")",};
    }

    inline static tuple<string, set<string>> createPatterns1() {
        string exactPattern = "((x+10)*(y-z)/5)";
        set<string> uniquePatterns {
            "x", "10", "x+10", "(x+10)",
            "y", "z", "y-z", "(y-z)",
            "(x+10)*(y-z)", "5", "(x+10)*(y-z)/5", "((x+10)*(y-z)/5)",
        };
        return make_pair(exactPattern, uniquePatterns);
    }
};

TEST_CASE("PatternTable: createPostfix -> createPatterns") {
    vector<string> exprList = TestPatterns::createExprList1();
    vector<string> postfix = PatternTable::createPostfix(exprList);

    tuple<string,set<string>> testPatterns = TestPatterns::createPatterns1();
    string testExactPattern = get<0>(testPatterns);
    set<string> testUniquePatterns = get<1>(testPatterns);

    tuple<string,set<string>> actualPatterns = PatternTable::createPatterns(postfix);
    string actualExactPattern = get<0>(actualPatterns);
    set<string> actualUniquePatterns = get<1>(actualPatterns);

    REQUIRE(testExactPattern == actualExactPattern);
    REQUIRE(testUniquePatterns.size() == actualUniquePatterns.size());

    for (auto &p : actualUniquePatterns) {
        testUniquePatterns.erase(p);
    }

    REQUIRE(testUniquePatterns.empty());
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
