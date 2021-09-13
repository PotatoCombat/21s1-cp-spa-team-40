#include <string>
#include <vector>

#include "pkb/StatementTable.h"

#include "catch.hpp"

using namespace std;

struct TestStatementTable {
public:
    static vector<Statement> createItems();
    static vector<StmtIndex> createIndices();
    static StatementTable createTable();
};

vector<Statement> TestStatementTable::createItems() {
    return {
        Statement(1, StatementType::ASSIGN),
        Statement(2, StatementType::READ),
        Statement(3, StatementType::PRINT),
    };
}

vector<StmtIndex> TestStatementTable::createIndices() { return vector<int>{1, 2, 3}; }

StatementTable TestStatementTable::createTable() {
    StatementTable table;

    auto items = TestStatementTable::createItems();
    for (Statement i : items) {
        table.insert(&i);
    }
    return table;
}

TEST_CASE("StatementTable: ctor") {
    StatementTable table;
    REQUIRE(table.getSize() == 0);
}

TEST_CASE("StatementTable: insert/getStmt/getIndex") {
    StatementTable table;
    Statement test = Statement(1, StatementType::CALL);
    table.insert(&test);

    REQUIRE(table.getSize() == 1);
    REQUIRE(table.getIndex(&test) == 1);
    REQUIRE(table.getStmt(1) == &test);
    REQUIRE(table.getStmt(1)->getIndex() == 1);
    REQUIRE(table.getStmtType(1) == StatementType::CALL);
}

TEST_CASE("EntityTable: invalid Statement") {
    auto table = TestStatementTable::createTable();
    REQUIRE(table.getStmt(4) == NULL);
}

TEST_CASE("EntityTable: invalid Index") {
    auto table = TestStatementTable::createTable();
    Statement test = Statement(4, StatementType::CALL);
    REQUIRE(table.getIndex(&test) == InvalidIndex);
}

TEST_CASE("StatementTable: getIndices") {
    auto table = TestStatementTable::createTable();

    vector<int> test = TestStatementTable::createIndices();
    vector<int> actual = table.getIndices().asVector();

    for (int i = 0; i < actual.size(); i++) {
        REQUIRE(test.at(i) == actual.at(i));
    }
}

TEST_CASE("StatementTable: getSize") {
    auto table = TestStatementTable::createTable();
    REQUIRE(table.getSize() == TestStatementTable::createItems().size());
}
