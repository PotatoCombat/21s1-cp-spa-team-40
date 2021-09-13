#include <string>
#include <vector>

#include "pkb/EntityTable.h"

#include "catch.hpp"

using namespace std;

struct TestEntity {
public:
    inline explicit TestEntity(string name) { this->name = name; }

    inline string getName() { return name; }

private:
    string name;
};

struct TestEntityTable {
public:
    inline static vector<TestEntity> createItems() {
        return vector<TestEntity>{
            TestEntity("hello"),
            TestEntity("goodbye"),
            TestEntity("sayonara")};
    }

    inline static EntityTable<TestEntity, string> createTable() {
        EntityTable<TestEntity, string> table;
        for (auto i : TestEntityTable::createItems()) {
            table.insert(&i);
        }
        return table;
    }
};

TEST_CASE("EntityTable: ctor") {
    EntityTable<TestEntity, string> table;
    REQUIRE(table.getSize() == 0);
}

TEST_CASE("EntityTable: insert/getEntity") {
    EntityTable<TestEntity, string> table;
    TestEntity test("hello");
    table.insert(&test);

    REQUIRE(table.getSize() == 1);
    REQUIRE(table.getEntity("hello") == &test);
}

TEST_CASE("EntityTable: entity not stored in table") {
    auto table = TestEntityTable::createTable();
    REQUIRE(table.getEntity("goodbye") == nullptr);
}

TEST_CASE("EntityTable: getNames") {
    auto table = TestEntityTable::createTable();

    vector<TestEntity> test = TestEntityTable::createItems();
    vector<string> actual = table.getNames().asVector();

    for (int i = 0; i < actual.size(); i++) {
        REQUIRE(test.at(i).getName() == actual.at(i));
    }
}

TEST_CASE("EntityTable: getSize") {
    auto table = TestEntityTable::createTable();
    REQUIRE(table.getSize() == TestEntityTable::createItems().size());
}
