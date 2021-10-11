#include <set>
#include <vector>

#include "pkb/NextTable.h"

#include "catch.hpp"

using namespace std;

struct TestNextTable {
    inline static bool equalVectors(vector<ProgLineIndex> actual, vector<ProgLineIndex> expected) {
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

    inline static bool equalSets(set<ProgLineIndex> actual, set<ProgLineIndex> expected) {
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

    // Sample based on https://github.com/nus-cs3203/project-wiki/wiki/
    // Full-(Basic-and-Advanced)-SPA-requirements#figure-5-cfg-for-procedure-second-in-code-5
    inline static NextTable createTable() {
        vector<Statement *> stmts;

        for (int i = 0; i <= 12; i++) {
            stmts.push_back(new Statement(i, StatementType::ASSIGN));
        }

        NextTable table;
        table.insertNext(stmts[1], stmts[2]);
        table.insertNext(stmts[2], stmts[3]);
        table.insertNext(stmts[3], stmts[4]);
        table.insertNext(stmts[4], stmts[5]);
        table.insertNext(stmts[5], stmts[6]);
        table.insertNext(stmts[6], stmts[3]);
        table.insertNext(stmts[3], stmts[7]);
        table.insertNext(stmts[7], stmts[8]);
        table.insertNext(stmts[7], stmts[9]);
        table.insertNext(stmts[8], stmts[10]);
        table.insertNext(stmts[9], stmts[10]);
        table.insertNext(stmts[10], stmts[11]);
        table.insertNext(stmts[11], stmts[12]);

        delete stmts[0]; // O index was dummy statement
        return table;
    }
};

TEST_CASE("NextTable: getNextLines") {
    NextTable table = TestNextTable::createTable();

    REQUIRE(TestNextTable::equalSets(table.getNextLines(1), { 2, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(2), { 3, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(3), { 4, 7, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(4), { 5, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(5), { 6, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(6), { 3, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(7), { 8, 9, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(8), { 10, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(9), { 10, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(10), { 11, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(11), { 12, }));
    REQUIRE(TestNextTable::equalSets(table.getNextLines(12), { }));
}

TEST_CASE("NextTable: getPreviousLines") {
    NextTable table = TestNextTable::createTable();

    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(1), { }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(2), { 1, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(3), { 2, 6, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(4), { 3, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(5), { 4, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(6), { 5, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(7), { 3, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(8), { 7, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(9), { 7, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(10), { 8, 9, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(11), { 10, }));
    REQUIRE(TestNextTable::equalSets(table.getPreviousLines(12), { 11, }));
}

TEST_CASE("NextTable: next") {
    NextTable table = TestNextTable::createTable();

    REQUIRE(table.next(1, 2));
    REQUIRE(table.next(2, 3));
    REQUIRE(table.next(3, 4));
    REQUIRE(table.next(4, 5));
    REQUIRE(table.next(5, 6));
    REQUIRE(table.next(6, 3));
    REQUIRE(table.next(3, 7));
    REQUIRE(table.next(7, 8));
    REQUIRE(table.next(7, 9));
    REQUIRE(table.next(8, 10));
    REQUIRE(table.next(9, 10));
    REQUIRE(table.next(10, 11));
    REQUIRE(table.next(11, 12));
}
