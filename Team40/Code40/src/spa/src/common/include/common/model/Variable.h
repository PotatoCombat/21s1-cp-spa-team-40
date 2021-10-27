#pragma once

#include "Abstractions.h"
#include "Entity.h"

using namespace std;

class Variable : public Entity<VarName> {
public:
    explicit Variable(VarName name);

    bool operator<(const Variable &other) const;
    bool operator==(const Variable &other) const;
};
