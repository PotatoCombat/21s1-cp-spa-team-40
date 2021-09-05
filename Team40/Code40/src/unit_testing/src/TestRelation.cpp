#include "catch.hpp"

#include "query_processor/query_preprocessor/pql_model/relation/Relation.h"
#include "query_processor/query_preprocessor/pql_model/relation/FollowsRelation.h"

struct TestRelation {
	static const Reference R1;
	static const Reference R2;
	static FollowsRelation createFollowsRelation();
};

const Reference TestRelation::R1 = Reference("s");
const Reference TestRelation::R2 = Reference("_");

FollowsRelation TestRelation::createFollowsRelation() {
	return FollowsRelation(R1, R2);
}

TEST_CASE("QP-Relation: contructor, getReference") {

	SECTION("test follows relation") {
		FollowsRelation rel = TestRelation::createFollowsRelation();

		REQUIRE(rel.getFirstReference() == TestRelation::R1);
		REQUIRE(rel.getSecondReference() == TestRelation::R2);
	}
}

TEST_CASE("QP-Relation: getRelationType") {
	Relation* rel;
	FollowsRelation followsR = TestRelation::createFollowsRelation();
	rel = &followsR;

	REQUIRE(rel->getRelationType() == RelationType::FOLLOWS);
	REQUIRE(rel->getRelationType() != RelationType::FOLLOWS_T);
}
