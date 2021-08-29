using namespace std;

#include "Relationship.h"

class FollowsRelationship : public Relationship {
	using Relationship::Relationship;
public:
	RelationshipType getRelationshipType() {
		return RelationshipType::FOLLOWS;
	}
};
