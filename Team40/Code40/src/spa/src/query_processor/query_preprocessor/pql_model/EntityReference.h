#pragma once

#include "Reference.h"

using namespace std;

// SYNONYM / UNDERSCORE / NAMED SYNONYM
class EntityReference : public Reference {
public:
    EntityReference(string value) : Reference(value) {}
    ReferenceType getType() { return ReferenceType::ENT_REF; }
};
