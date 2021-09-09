#pragma once

#include <string>
#include <vector>

using namespace std;

typedef string DeclType;
typedef string DeclValue;

typedef string RelType;
typedef string RelArg;

typedef string PatIdent;
typedef string PatArg;

typedef pair<DeclType, DeclValue> DeclTuple;
typedef tuple<RelType, RelArg, RelArg> RelTuple;
typedef pair<PatIdent, vector<PatArg>> PatTuple;