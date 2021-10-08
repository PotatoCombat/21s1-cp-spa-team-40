#include <set>
#include <vector>

#include "pkb/CallsTable.h"

#include "catch.hpp"

#define EQUALS_SET( actual, ... ) REQUIRE(TestCallsTable::equalSets(actual, __VA_ARGS__))

using namespace std;

struct TestCallsTable {
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
    inline static CallsTable createTable() {
        vector<Procedure *> procs;

        vector<ProcName> procNames = createProcNames(6);
        for (auto &procName : procNames) {
            procs.push_back(new Procedure(procName));
        }

        CallsTable table;
        table.insertCalls(procs[1], procs[2]->getName());
        table.insertCalls(procs[2], procs[3]->getName());
        table.insertCalls(procs[3], procs[4]->getName());
        table.insertCalls(procs[2], procs[5]->getName());
        table.insertCalls(procs[5], procs[3]->getName());
        table.insertCalls(procs[5], procs[4]->getName());
        table.insertCalls(procs[5], procs[6]->getName());
        table.insertCalls(procs[6], procs[3]->getName());
        table.insertCalls(procs[6], procs[4]->getName());

        delete procs[0]; // O index was dummy procedure
        return table;
    }
};

TEST_CASE("CallsTable: getCalledProcs") {
    CallsTable table = TestCallsTable::createTable();
    vector<ProcName> p = TestCallsTable::createProcNames(6);

    EQUALS_SET(table.getCalledProcs(p[1]), { p[2], });
    EQUALS_SET(table.getCalledProcs(p[2]), { p[3], p[5], });
    EQUALS_SET(table.getCalledProcs(p[3]), { p[4], });
    EQUALS_SET(table.getCalledProcs(p[4]), { });
    EQUALS_SET(table.getCalledProcs(p[5]), { p[3], p[4], p[6], });
    EQUALS_SET(table.getCalledProcs(p[6]), { p[3], p[4], });
}

TEST_CASE("CallsTable: getCallerProcs") {
    CallsTable table = TestCallsTable::createTable();
    vector<ProcName> p = TestCallsTable::createProcNames(6);

    EQUALS_SET(table.getCallerProcs(p[1]), { });
    EQUALS_SET(table.getCallerProcs(p[2]), { p[1], });
    EQUALS_SET(table.getCallerProcs(p[3]), { p[2], p[5], p[6], });
    EQUALS_SET(table.getCallerProcs(p[4]), { p[3], p[5], p[6], });
    EQUALS_SET(table.getCallerProcs(p[5]), { p[2], });
    EQUALS_SET(table.getCallerProcs(p[6]), { p[5], });
}

TEST_CASE("CallsTable: calls") {
    CallsTable table = TestCallsTable::createTable();
    vector<ProcName> p = TestCallsTable::createProcNames(6);

    REQUIRE(table.calls(p[1], p[2]));
    REQUIRE(table.calls(p[2], p[3]));
    REQUIRE(table.calls(p[3], p[4]));
    REQUIRE(table.calls(p[2], p[5]));
    REQUIRE(table.calls(p[5], p[3]));
    REQUIRE(table.calls(p[5], p[4]));
    REQUIRE(table.calls(p[5], p[6]));
    REQUIRE(table.calls(p[6], p[3]));
    REQUIRE(table.calls(p[6], p[4]));
}
