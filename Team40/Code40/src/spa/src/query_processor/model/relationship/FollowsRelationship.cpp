#include "Relationship.h"

using namespace std;

class FollowsRelationship : public Relationship {
    using Relationship::Relationship;

public:
    RelationshipType GetRelationshipType() { return RelationshipType::FOLLOWS; }
};
