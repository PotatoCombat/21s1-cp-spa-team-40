#include <set>
#include <vector>

#include "pkb/CallsStarTable.h"

#include "catch.hpp"

#define EQUALS_SET( actual, ... ) REQUIRE(TestCallsStarTable::equalSets(actual, __VA_ARGS__))

using namespace std;

struct TestCallsStarTable {
    inline static bool equalSets(set<ProcName> actual, set<ProcName> expected) {
        if (actual.size() != expected.size()) {
            return false;
        }
        set<ProcName> matches(actual.begin(), actual.end());
        for (auto &s : expected) {
            auto keyValuePair = matches.find(s);
            if (keyValuePair == matches.end()) {
                return false;
            }
        }
        return true;
    }
    
    inline static vector<ProcName> createProcNames(int size) {
        vector<ProcName> procNames;
        for (int i = 0; i <= size; i++) {
            procNames.push_back("Procedure" + to_string(i));
        }
        return procNames;
    }

    // Sample based on https://github.com/nus-cs3203/project-wiki/wiki/
    // Full-(Basic-and-Advanced)-SPA-requirements#figure-5-cfg-for-procedure-second-in-code-5
    inline static CallsStarTable createTable() {
        vector<Procedure *> procs;

        vector<ProcName> procNames = createProcNames(6);
        for (auto &procName : procNames) {
            procs.push_back(new Procedure(procName));
        }

        CallsStarTable table;
        table.insertCallsStar(procs[1], procs[2]->getName());
        table.insertCallsStar(procs[2], procs[3]->getName());
        table.insertCallsStar(procs[3], procs[4]->getName());
        table.insertCallsStar(procs[2], procs[5]->getName());
        table.insertCallsStar(procs[5], procs[3]->getName());
        table.insertCallsStar(procs[5], procs[4]->getName());
        table.insertCallsStar(procs[5], procs[6]->getName());
        table.insertCallsStar(procs[6], procs[3]->getName());
        table.insertCallsStar(procs[6], procs[4]->getName());

        delete procs[0]; // O index was dummy procedure
        return table;
    }
};

TEST_CASE("CallsStarTable: getCalledStarProcs") {
    CallsStarTable table = TestCallsStarTable::createTable();
    vector<ProcName> p = TestCallsStarTable::createProcNames(6);

    EQUALS_SET(table.getCalledStarProcs(p[1]), { p[2], p[3], p[4], p[5], p[6], });
    EQUALS_SET(table.getCalledStarProcs(p[2]), { p[3], p[4], p[5], p[6], });
    EQUALS_SET(table.getCalledStarProcs(p[3]), { p[4], });
    EQUALS_SET(table.getCalledStarProcs(p[4]), { });
    EQUALS_SET(table.getCalledStarProcs(p[5]), { p[3], p[4], p[6], });
    EQUALS_SET(table.getCalledStarProcs(p[6]), { p[3], p[4], });
}

TEST_CASE("CallsStarTable: getCallerStarProcs") {
    CallsStarTable table = TestCallsStarTable::createTable();
    vector<ProcName> p = TestCallsStarTable::createProcNames(6);

    EQUALS_SET(table.getCallerStarProcs(p[1]), { });
    EQUALS_SET(table.getCallerStarProcs(p[2]), { p[1], });
    EQUALS_SET(table.getCallerStarProcs(p[3]), { p[1], p[2], p[5], p[6], });
    EQUALS_SET(table.getCallerStarProcs(p[4]), { p[1], p[2], p[3], p[5], p[6], });
    EQUALS_SET(table.getCallerStarProcs(p[5]), { p[1], p[2], });
    EQUALS_SET(table.getCallerStarProcs(p[6]), { p[1], p[2], p[5], });
}

TEST_CASE("CallsStarTable: callsStar") {
    CallsStarTable table = TestCallsStarTable::createTable();
    vector<ProcName> p = TestCallsStarTable::createProcNames(6);

    REQUIRE(table.callsStar(p[1], p[2]));
    REQUIRE(table.callsStar(p[1], p[3]));
    REQUIRE(table.callsStar(p[1], p[4]));
    REQUIRE(table.callsStar(p[1], p[5]));
    REQUIRE(table.callsStar(p[1], p[6]));
    REQUIRE(table.callsStar(p[2], p[3]));
    REQUIRE(table.callsStar(p[2], p[4]));
    REQUIRE(table.callsStar(p[2], p[5]));
    REQUIRE(table.callsStar(p[2], p[6]));
    REQUIRE(table.callsStar(p[3], p[4]));
    REQUIRE(table.callsStar(p[5], p[3]));
    REQUIRE(table.callsStar(p[5], p[4]));
    REQUIRE(table.callsStar(p[5], p[6]));
    REQUIRE(table.callsStar(p[6], p[3]));
    REQUIRE(table.callsStar(p[6], p[4]));
}
