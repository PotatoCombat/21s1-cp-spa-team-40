#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "pkb/PatternTable.h"

#include "catch.hpp"

using namespace std;

struct TestPatterns {
    inline static vector<string> createExprList1() {
        // ((x + 10) * (y - z) / 5)
        return vector<string>{"(", "(", "x", "+", "10", ")", "*", "(", "y", "-", "z", ")", "/", "5", ")"};
    }

    inline static set<string> createPatternList1() {
        return set<string>{
            "x", "10", "x+10", "(x+10)",
            "y", "z", "y-z", "(y-z)",
            "(x+10)*(y-z)", "5", "(x+10)*(y-z)/5", "((x+10)*(y-z)/5)"};
    }
};

TEST_CASE("PatternTable: createPostfix -> createPatterns") {
    vector<string> exprList = TestPatterns::createExprList1();
    vector<string> postfix = PatternTable::createPostfix(exprList);

    set<string> testPatterns = TestPatterns::createPatternList1();
    set<string> actualPatterns = PatternTable::createPatterns(postfix);

    REQUIRE(testPatterns.size() == actualPatterns.size());

    for (auto &s : actualPatterns) {
        testPatterns.erase(s);
    }

    REQUIRE(testPatterns.empty());
}

//TEST_CASE("PatternTable: ctor") {
//    PatternTable table;
//    REQUIRE(table.getSize() == 0);
//}
//
//TEST_CASE("PatternTable: insert/getEntity") {
//    EntityTable<TestEntity, string> table;
//    TestEntity test("hello");
//    table.insert(&test);
//
//    REQUIRE(table.getSize() == 1);
//    REQUIRE(table.getEntity("hello") == &test);
//}
//
//TEST_CASE("EntityTable: entity not stored in table") {
//    auto table = TestEntityTable::createTable();
//    REQUIRE(table.getEntity("goodbye") == nullptr);
//}
//
//TEST_CASE("EntityTable: getNames") {
//    auto table = TestEntityTable::createTable();
//
//    vector<TestEntity> test = TestEntityTable::createItems();
//    vector<string> actual = table.getNames().asVector();
//
//    for (int i = 0; i < actual.size(); i++) {
//        REQUIRE(test.at(i).getName() == actual.at(i));
//    }
//}
//
//TEST_CASE("EntityTable: getSize") {
//    auto table = TestEntityTable::createTable();
//    REQUIRE(table.getSize() == TestEntityTable::createItems().size());
//}
