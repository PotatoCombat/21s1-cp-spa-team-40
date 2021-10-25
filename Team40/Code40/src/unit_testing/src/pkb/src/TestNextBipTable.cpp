#include <set>
#include <vector>

#include "pkb/NextBipTable.h"

#include "catch.hpp"

using namespace std;

struct TestNextBipTable {
    inline static bool equalVectors(vector<ProgLineIndex> actual,
                                    vector<ProgLineIndex> expected) {
        if (actual.size() != expected.size()) {
            return false;
        }
        set<ProgLineIndex> matches(actual.begin(), actual.end());
        for (auto &s : expected) {
            auto keyValuePair = matches.find(s);
            if (keyValuePair == matches.end()) {
                return false;
            }
        }
        return true;
    }

    inline static bool equalSets(set<ProgLineIndex> actual,
                                 set<ProgLineIndex> expected) {
        if (actual.size() != expected.size()) {
            return false;
        }
        set<ProgLineIndex> matches(actual.begin(), actual.end());
        for (auto &s : expected) {
            auto keyValuePair = matches.find(s);
            if (keyValuePair == matches.end()) {
                return false;
            }
        }
        return true;
    }

    // Sample based on
    // https://github.com/nus-cs3203/project-wiki/wiki/Project-Iterations-2-and-3
    // #appendix-b-extension-definition
    inline static NextBipTable createTable() {
        vector<Statement *> stmts;

        for (int i = 0; i <= 12; i++) {
            stmts.push_back(new Statement(i, StatementType::ASSIGN));
        }

        NextBipTable table;
        table.insertNextBip(stmts[1], stmts[2]);
        table.insertNextBip(stmts[2], stmts[6]);
        table.insertNextBip(stmts[6], stmts[7]);
        table.insertNextBip(stmts[7], stmts[9]);
        table.insertNextBip(stmts[9], stmts[10]);
        table.insertNextBip(stmts[9], stmts[11]);
        table.insertNextBip(stmts[10], stmts[8]);
        table.insertNextBip(stmts[11], stmts[8]);
        table.insertNextBip(stmts[8], stmts[3]);
        table.insertNextBip(stmts[3], stmts[4]);
        table.insertNextBip(stmts[4], stmts[5]);

        delete stmts[0]; // O index was dummy statement
        return table;
    }
};

TEST_CASE("NextBipTable: getNextBipLines") {
    NextBipTable table = TestNextBipTable::createTable();

    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(1), {2}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(2), {6}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(6), {7}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(7), {9}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(9), {10, 11}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(10), {8}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(11), {8}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(8), {3}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(3), {4}));
    REQUIRE(TestNextBipTable::equalSets(table.getNextBipLines(4), {5}));
}

TEST_CASE("NextBipTable: getPreviousBipLines") {
    NextBipTable table = TestNextBipTable::createTable();

    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(1), {}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(2), {1}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(6), {2}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(7), {6}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(9), {7}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(10), {9}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(11), {9}));
    REQUIRE(
        TestNextBipTable::equalSets(table.getPreviousBipLines(8), {10, 11}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(3), {8}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(4), {3}));
    REQUIRE(TestNextBipTable::equalSets(table.getPreviousBipLines(5), {4}));
}

TEST_CASE("NextBipTable: nextBip") {
    NextBipTable table = TestNextBipTable::createTable();

    REQUIRE(table.nextBip(1, 2));
    REQUIRE(table.nextBip(2, 6));
    REQUIRE(table.nextBip(6, 7));
    REQUIRE(table.nextBip(7, 9));
    REQUIRE(table.nextBip(9, 10));
    REQUIRE(table.nextBip(9, 11));
    REQUIRE(table.nextBip(10, 8));
    REQUIRE(table.nextBip(11, 8));
    REQUIRE(table.nextBip(8, 3));
    REQUIRE(table.nextBip(3, 4));
    REQUIRE(table.nextBip(4, 5));
}
