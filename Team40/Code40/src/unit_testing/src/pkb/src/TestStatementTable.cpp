#include <string>
#include <vector>

#include "pkb/StatementTable.h"

#include "catch.hpp"

using namespace std;

struct TestStatementTable {
public:
    static vector<Statement> createItems() {
        return {
            Statement(1, StatementType::ASSIGN),
            Statement(2, StatementType::READ),
            Statement(3, StatementType::PRINT),
            };
    }
};

TEST_CASE("StatementTable") {
    StatementTable table;

    // Check Size of empty table
    REQUIRE(table.getSize() == 0);

    vector<Statement> items = TestStatementTable::createItems();

    // Insert one by one to get reference in items
    table.insert(&items[0]);
    table.insert(&items[1]);
    table.insert(&items[2]);

    // Check Size
    REQUIRE(table.getSize() == items.size());

    REQUIRE(*table.getStmt(items[0].getId()) == items[0]);
    REQUIRE(*table.getStmt(items[1].getId()) == items[1]);
    REQUIRE(*table.getStmt(items[2].getId()) == items[2]);

    // Check NONE References
    REQUIRE(table.getStmt(4) == nullptr);

    // Check Names
    vector<StmtIndex> indices = table.getIndices().asVector();
    REQUIRE(items.size() == indices.size());

    // TODO: Will fix after replacing Iterator with set
    REQUIRE(count(indices.begin(), indices.end(), items[0].getId()) == 1);
    REQUIRE(count(indices.begin(), indices.end(), items[1].getId()) == 1);
    REQUIRE(count(indices.begin(), indices.end(), items[2].getId()) == 1);
}
