#include <map>
#include <vector>

#include "Abstractions.h"
#include "PKB.h"

#include "catch.hpp"

using namespace std;

struct TestPKB {
public:
    static vector<string> createItems();
    static vector<int> createIndices();
};

vector<string> TestPKB::createItems() {
    return vector<string> {"hello", "goodbye", "sayonara"};
}

vector<int> TestPKB::createIndices() {
    return vector<int> {0, 1, 2};
}

TEST_CASE("PKB: ctor") {
    PKB pkb;
    REQUIRE(pkb.getAllVars().empty());
    REQUIRE(pkb.getAllConsts().empty());
    REQUIRE(pkb.getAllProcs().empty());
    REQUIRE(pkb.getAllStmts().empty());
}

TEST_CASE("PKB: insertVar/getAllVars") {
    PKB pkb;

    vector<Var> items = TestPKB::createItems();
    for (const auto& i : items)
    {
        pkb.insertVar(i);
    }

    vector<VarIndex> test = TestPKB::createIndices();
    vector<VarIndex> actual = pkb.getAllVars();

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}

TEST_CASE("PKB: insertConst/getAllConsts") {
    PKB pkb;

    vector<Const> items = TestPKB::createItems();
    for (const auto& i : items)
    {
        pkb.insertConst(i);
    }

    vector<ConstIndex> test = TestPKB::createIndices();
    vector<ConstIndex> actual = pkb.getAllConsts();

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}

TEST_CASE("PKB: insertProcs/getAllProcs") {
    PKB pkb;

    vector<Procedure> items = TestPKB::createItems();
    for (const auto& i : items)
    {
        pkb.insertProc(i);
    }

    vector<ProcedureIndex> test = TestPKB::createIndices();
    vector<ProcedureIndex> actual = pkb.getAllProcs();

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}
