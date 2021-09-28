#pragma once

#include <string>
#include <vector>

using namespace std;

typedef string DeclType;
typedef string DeclValue;

typedef string ClsType;
typedef string ClsArg;

typedef string PatIdent;
typedef string PatArg;

typedef string WithAttr; // or int?
typedef string WithArg;

typedef pair<DeclType, DeclValue> DeclPair;
typedef tuple<ClsType, ClsArg, ClsArg> ClsTuple;
typedef tuple<PatIdent, PatArg, PatArg> PatTuple;
typedef tuple<WithAttr, WithArg, WithArg> WithTuple;
