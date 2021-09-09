#include "catch.hpp"

#include "Reference.h"
#include "Relation.h"

using namespace std;

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

TEST_CASE("Relation: equals") {
    Reference ref1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference ref2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference ref3(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Reference ref4(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "x");
    Reference ref5(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "x");
    Reference ref6(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "x");

    // all match
    Relation rela1(RelationType::USES_S, &ref1, &ref4);
    Relation rela2(RelationType::USES_S, &ref2, &ref5);
    REQUIRE(rela1.equals(rela2));
    // different type
    Relation rela3(RelationType::MODIFIES_S, &ref1, &ref4);
    Relation rela4(RelationType::USES_S, &ref2, &ref5);
    REQUIRE(!rela3.equals(rela4));
    // different first reference
    Relation rela5(RelationType::USES_S, &ref1, &ref4);
    Relation rela6(RelationType::USES_S, &ref3, &ref5);
    REQUIRE(!rela5.equals(rela6));
    // different second reference
    Relation rela7(RelationType::USES_S, &ref1, &ref4);
    Relation rela8(RelationType::USES_S, &ref2, &ref6);
    REQUIRE(!rela7.equals(rela8));
}
