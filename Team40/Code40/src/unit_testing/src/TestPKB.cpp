#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "PKB.h"

#include "catch.hpp"

using namespace std;

PKB createPKB() {
    PKB pkb;

    pkb.insertVar("hello");
    pkb.insertVar("goodbye");
    pkb.insertVar("sayonara");

    return pkb;
}

TEST_CASE("PKB VarTable - insert/get") {
    PKB pkb = createPKB();

    vector<Var> test {"hello", "goodbye", "sayonara"};
    vector<Var> actual = pkb.getAllVars();

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}

TEST_CASE("PKB ConstTable - insert/get") {
    PKB pkb = createPKB();

    vector<Const> test {"hello", "goodbye", "sayonara"};
    vector<Const> actual = pkb.getAllConsts();

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}

TEST_CASE("PKB ProcedureTable - insert/get") {
    PKB pkb = createPKB();

    vector<Procedure> test {"hello", "goodbye", "sayonara"};
    vector<Procedure> actual = pkb.getAllProcs();

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}
