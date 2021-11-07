#include "catch.hpp"

#include <set>

#include "TestPKBUtils.h"

#include "common/model/Abstractions.h"

#include "pkb/RelationshipTable.h"

#define EQUALS_SET( actual, ... ) REQUIRE(TestUtils::equalSets(actual, __VA_ARGS__))

using namespace std;

TEST_CASE("RelationshipTable: standard table") {
    RelationshipTable<int, int> table;

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
