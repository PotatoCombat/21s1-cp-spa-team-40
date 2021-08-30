#include <string>
#include <map>

#include "PKB.h"
#include "EntityTable.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("Tables") {
    typedef EntityTable<Var, VarIndex> VarTable;
    VarTable varTable;
    varTable.insert("hello");
    varTable.insert("goodbye");
    varTable.insert("sayonara");

    vector<Var> test {"hello", "goodbyes", "sayonara"};
    vector<Var> actual = varTable.getEntities();

    for (int i = 0; i < varTable.getSize(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}
