#pragma once

#include "EntityReference.h"

class NamedEntity : public EntityReference {
public:
    NamedEntity(string value, DesignEntityType type)
        : EntityReference(value, type) {}
    bool isSynonym() { return false; }
};
