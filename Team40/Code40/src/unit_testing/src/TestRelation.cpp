#include "catch.hpp"

#include "query_processor/query_preprocessor/pql_model/relation/Relation.h"
#include "query_processor/query_preprocessor/pql_model/relation/FollowsRelation.h"

Reference ref1("reference");
Reference ref2("another");
FollowsRelation followsR(ref1, ref2);

TEST_CASE("QP-FollowsRelation: contructor, getReference") {
	REQUIRE(followsR.getFirstReference().getValue() == ref1.getValue());
	REQUIRE(followsR.getSecondReference().getValue() == ref2.getValue());
}

TEST_CASE("QP-FollowsRelation: getRelationType") {
	Relation* rel;
	rel = &followsR;
	REQUIRE(rel->getRelationType() == RelationType::FOLLOWS);
}
