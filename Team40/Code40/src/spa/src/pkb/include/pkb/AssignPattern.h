#pragma once

#include <string>
#include "common/model/Abstractions.h"

using namespace std;

struct AssignPattern {
    VarName var;
    string pattern;

    bool operator==(const AssignPattern& other) const {
        return var == other.var && pattern == other.pattern;
    }

    bool operator<(const AssignPattern& other) const {
        return var < other.var
            || (var == other.var && pattern < other.pattern);
    }
};
