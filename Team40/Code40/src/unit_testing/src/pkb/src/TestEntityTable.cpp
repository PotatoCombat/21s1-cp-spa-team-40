#include <string>
#include <vector>

#include "pkb/include/EntityTable.h"

#include "catch.hpp"

using namespace std;

struct TestEntityTable {
public:
    static vector<string> createItems();
    static vector<int> createIndices();
    static EntityTable<string, int> createTable();
};

vector<string> TestEntityTable::createItems() {
    return vector<string> {"hello", "goodbye", "sayonara"};
}

vector<int> TestEntityTable::createIndices() {
    return vector<int> {0, 1, 2};
}

EntityTable<string, int> TestEntityTable::createTable() {
    EntityTable<string, int> table;

    auto items = TestEntityTable::createItems();
    for (const auto& i : items)
    {
        table.insert(i);
    }
    return table;
}

TEST_CASE("EntityTable: ctor") {
    EntityTable<string, int> table;
    REQUIRE(table.getSize() == 0);
}

TEST_CASE("EntityTable: insert/getEntity/getIndex") {
    EntityTable<string, int> table;
    table.insert("hello");

    REQUIRE(table.getSize() == 1);
    REQUIRE(table.getIndex("hello") == 0);
    REQUIRE(table.getEntity(0) == "hello");
}

TEST_CASE("EntityTable: getIndices") {
    auto table = TestEntityTable::createTable();

    vector<int> test = TestEntityTable::createIndices();
    vector<int> actual = table.getIndices().asVector();

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}

TEST_CASE("EntityTable: getSize") {
    auto table = TestEntityTable::createTable();
    REQUIRE(table.getSize() == TestEntityTable::createItems().size());
}
