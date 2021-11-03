#pragma once

#include "Abstractions.h"
#include "Entity.h"

using namespace std;

class ConstantValue : public Entity<ConstName> {
public:
    explicit ConstantValue(ConstName name);

    bool operator<(const ConstantValue &other) const;
    bool operator==(const ConstantValue &other) const;
};
