#include "Relationship.h"

using namespace std;

class FollowsRelationship : public Relationship {
    using Relationship::Relationship;

public:
    RelationshipType getRelationshipType() { return RelationshipType::FOLLOWS; }
};
