#include <string>
#include <utility>
#include <vector>

#include "pkb/EntityTable.h"

#include "catch.hpp"

using namespace std;

class TestEntity : public Entity<string> {
public:
    explicit TestEntity(string name) : Entity<string>(move(name)) { };

    bool operator<(const TestEntity &other) const {
        return id < other.id;
    }

    bool operator==(const TestEntity &other) const {
        return id == other.id;
    }
};

struct TestEntityTable {
public:
    static vector<TestEntity> createItems() {
        return {
            TestEntity("hello"),
            TestEntity("goodbye"),
            TestEntity("sayonara"),
        };
    }
};

TEST_CASE("EntityTable") {
    EntityTable<TestEntity, string> table;

    // Check Size of empty table
    REQUIRE(table.getSize() == 0);

    vector<TestEntity> items = TestEntityTable::createItems();

    // Insert one by one to get reference in items
    table.insert(&items[0]);
    table.insert(&items[1]);
    table.insert(&items[2]);

    // Check Size
    REQUIRE(table.getSize() == items.size());

    REQUIRE(*table.getEntity(items[0].getId()) == items[0]);
    REQUIRE(*table.getEntity(items[1].getId()) == items[1]);
    REQUIRE(*table.getEntity(items[2].getId()) == items[2]);

    // Check NONE References
    REQUIRE(table.getEntity("bye") == nullptr);

    // Check Names
    vector<string> names = table.getIds().asVector();
    REQUIRE(items.size() == names.size());

    // TODO: Will fix after replacing Iterator with set
    REQUIRE(count(names.begin(), names.end(), items[0].getId()) == 1);
    REQUIRE(count(names.begin(), names.end(), items[1].getId()) == 1);
    REQUIRE(count(names.begin(), names.end(), items[2].getId()) == 1);
}
