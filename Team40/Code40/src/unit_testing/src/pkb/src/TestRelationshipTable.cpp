#include <set>

#include "common/model/Abstractions.h"
#include "pkb/RelationshipTable.h"

#include "catch.hpp"

#define EQUALS_SET( actual, ... ) REQUIRE(TestRelationshipTable::equalSets(actual, __VA_ARGS__))

using namespace std;

struct TestRelationshipTable {
    typedef RelationshipTable<int, int> Table;

    inline static bool equalSets(set<int> actual, set<int> expected) {
        if (actual.size() != expected.size()) {
            return false;
        }
        set<int> matches(actual.begin(), actual.end());
        for (auto &s : expected) {
            auto keyValuePair = matches.find(s);
            if (keyValuePair == matches.end()) {
                return false;
            }
        }
        return true;
    }
};

TEST_CASE("RelationshipTable: standard table") {
    TestRelationshipTable::Table table;

    int L1 = 1;
    int R1 = 2;

    int L2 = 2;
    int R2 = 3;

    table.insertRelationship(L1, R1);

    REQUIRE(table.isRelationship(L1, R1));
    REQUIRE(table.getFirstRHSRelationship(L1, InvalidIndex) == R1);
    REQUIRE(table.getFirstLHSRelationship(R1, InvalidIndex) == L1);
    EQUALS_SET(table.getLHSRelationships(R1), { L1, });
    EQUALS_SET(table.getRHSRelationships(L1), { R1, });

    table.insertRelationship(L2, R2);

    REQUIRE(table.isRelationship(L1, R1));
    REQUIRE(table.isRelationship(L2, R2));
    REQUIRE(!table.isRelationship(L1, R2));
    REQUIRE(table.getFirstLHSRelationship(R1, InvalidIndex) == L1);
    REQUIRE(table.getFirstRHSRelationship(L1, InvalidIndex) == R1);
    EQUALS_SET(table.getLHSRelationships(R1), { L1, });
    EQUALS_SET(table.getLHSRelationships(R2), { L2, });
    EQUALS_SET(table.getRHSRelationships(L1), { R1, });
    EQUALS_SET(table.getRHSRelationships(L2), { R2, });

    table.updateTransitivity(L2, R2);

    REQUIRE(table.isRelationship(L1, R1));
    REQUIRE(table.isRelationship(L2, R2));
    REQUIRE(table.isRelationship(L1, R2));
    REQUIRE(table.getFirstLHSRelationship(R1, InvalidIndex) == L1);
    REQUIRE(table.getFirstRHSRelationship(L1, InvalidIndex) == R1);
    EQUALS_SET(table.getLHSRelationships(R1), { L1, });
    EQUALS_SET(table.getLHSRelationships(R2), { L1, L2, });
    EQUALS_SET(table.getRHSRelationships(L1), { R1, R2, });
    EQUALS_SET(table.getRHSRelationships(L2), { R2, });
}
