#pragma once

#include <string>
#include <vector>

using namespace std;

typedef string DeclType;
typedef string DeclValue;
typedef pair<DeclType, DeclValue> DeclPair;

typedef string ClsType;
typedef string ClsArg;
typedef tuple<ClsType, ClsArg, ClsArg> ClsTuple;

typedef string PatIdent;
typedef string PatArg;
typedef pair<PatIdent, vector<PatArg>> PatTuple;

typedef string WithArg;
typedef vector<WithArg> WithTuple; // always four args (for now)
