#pragma once

#include <string>

using namespace std;

typedef string ProcName;
typedef string VarName;
typedef string ConstName;

typedef int StmtIndex;
constexpr int InvalidIndex = -1;

typedef string Pattern;

template<class T> struct Comparator {
    bool operator()(const T* lhs, const T* rhs) const {
        return *lhs < *rhs;
    }
};
