#include "catch.hpp"

#include "query_processor/query_preprocessor/pql_model/clause/SuchThatClause.h"
#include "query_processor/query_preprocessor/pql_model/relation/FollowsRelation.h"

struct TestStClause {
    static const Reference REF1;
    static const Reference REF2;
    static const FollowsRelation REL;
    static SuchThatClause createClause();
};

const Reference TestStClause::REF1 = Reference("s");
const Reference TestStClause::REF2 = Reference("v");
const FollowsRelation TestStClause::REL =
    FollowsRelation(TestStClause::REF1, TestStClause::REF2);

SuchThatClause TestStClause::createClause() {
    return SuchThatClause(TestStClause::REL);
}

TEST_CASE("QP-SuchThatClause: constructor, getRelation") {
    SuchThatClause cl = TestStClause::createClause();

    REQUIRE(cl.getRelation().getFirstReference() == TestStClause::REF1);
    REQUIRE(cl.getRelation().getSecondReference() == TestStClause::REF2);
    REQUIRE(cl.getRelation().getRelationType() == RelationType::FOLLOWS);
}
