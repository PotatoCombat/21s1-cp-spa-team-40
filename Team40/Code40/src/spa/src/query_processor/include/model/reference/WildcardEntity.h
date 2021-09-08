#pragma once

#include "EntityReference.h"

class WildcardEntity : public EntityReference {
public:
    WildcardEntity(DesignEntityType type)
        : EntityReference("_", type) {} // or change to using a different ctor
    bool isSynonym() { return false; }
};
