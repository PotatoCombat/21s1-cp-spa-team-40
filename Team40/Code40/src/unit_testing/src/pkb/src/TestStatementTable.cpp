#include <string>
#include <vector>

#include "pkb/include/StatementTable.h"

#include "catch.hpp"

using namespace std;

struct TestStatementTable {
public:
    static vector<string> createItems();
    static vector<int> createIndices();
    static StatementTable<string, int> createTable();
};

vector<string> TestStatementTable::createItems() {
    return vector<string> {"hello", "goodbye", "sayonara"};
}

vector<int> TestStatementTable::createIndices() {
    return vector<int> {1, 2, 3};
}

StatementTable<string, int> TestStatementTable::createTable() {
    StatementTable<string, int> table;

    auto items = TestStatementTable::createItems();
    for (const auto& i : items)
    {
        table.insert(i);
    }
    return table;
}

TEST_CASE("StatementTable: ctor") {
    StatementTable<string, int> table;
    REQUIRE(table.getSize() == 0);
}

TEST_CASE("StatementTable: insert/getStmt/getIndex") {
    StatementTable<string, int> table;
    table.insert("hello");

    REQUIRE(table.getSize() == 1);
    REQUIRE(table.getIndex("hello") == 1);
    REQUIRE(table.getStmt(1) == "hello");
}

TEST_CASE("StatementTable: getIndices") {
    auto table = TestStatementTable::createTable();

    vector<int> test = TestStatementTable::createIndices();
    vector<int> actual = table.getIndices().asVector();

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(test.at(i) == actual.at(i));
    }
}

TEST_CASE("StatementTable: getSize") {
    auto table = TestStatementTable::createTable();
    REQUIRE(table.getSize() == TestStatementTable::createItems().size());
}
