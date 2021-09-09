#include "catch.hpp"

#include "Reference.h"
#include "Relation.h"

struct TestRelation {
    static Reference R1;
    static Reference R2;
    static Relation createFollowsRelation();
};

Reference TestRelation::R1 = Reference(
    DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
Reference TestRelation::R2 = Reference(
    DesignEntityType::STMT, ReferenceType::CONSTANT, "_");

Relation TestRelation::createFollowsRelation() {
    return Relation(RelationType::FOLLOWS, &R1, &R2);
}

TEST_CASE("Relation: get methods") {
    Relation rel = TestRelation::createFollowsRelation();

    REQUIRE(rel.getType() == RelationType::FOLLOWS);
    REQUIRE(TestRelation::R1.equals(*rel.getFirstReference()));
    REQUIRE(TestRelation::R2.equals(*rel.getSecondReference()));
}
