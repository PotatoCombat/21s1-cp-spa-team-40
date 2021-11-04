#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Entity.h"

using namespace std;

class Variable : public Entity<VarName> {
public:
    explicit Variable(VarName name);

    bool operator<(const Variable &other) const;
    bool operator==(const Variable &other) const;
};
