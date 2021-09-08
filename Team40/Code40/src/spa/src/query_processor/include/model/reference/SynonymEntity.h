#pragma once

#include "EntityReference.h"

class SynonymEntity : public EntityReference {
public:
    SynonymEntity(string value, DesignEntityType type)
        : EntityReference(value, type) {}
    bool isSynonym() { return true; }
};
