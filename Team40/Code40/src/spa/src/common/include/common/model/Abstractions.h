#pragma once

#include <string>
#include <vector>

using namespace std;

typedef string ProcName;
typedef string VarName;
typedef string ConstName;

typedef int StmtIndex;
typedef int ProgLineIndex;
constexpr int InvalidIndex = -1;

typedef string Pattern;
const string WILDCARD = "_";

typedef vector<string> ExpressionList;

template<class T> struct Comparator {
    bool operator()(const T* lhs, const T* rhs) const {
        return *lhs < *rhs;
    }
};
