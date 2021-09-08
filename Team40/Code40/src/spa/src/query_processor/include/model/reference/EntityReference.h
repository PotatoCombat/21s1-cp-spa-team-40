#pragma once

#include "Reference.h"

class EntityReference : public Reference {
public:
    EntityReference(string value, DesignEntityType type)
        : Reference(value, type) {}
};

