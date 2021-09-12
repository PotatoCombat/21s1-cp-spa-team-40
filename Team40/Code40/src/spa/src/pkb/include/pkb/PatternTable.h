#pragma once

#include <map>
#include <set>
#include <vector>

#include "Abstractions.h"
#include "Iterator.h"

#include "common/model/statement/AssignStatement.h"

using namespace std;

class PatternTable {
public:
    PatternTable();

    void insertAssignment(AssignStatement *a);
    set<StmtIndex> getStmtsWithPattern(VarName varName, string pattern);
    bool pattern(StmtIndex stmt, VarName varName, string pattern);

    // THESE METHODS WILL BE PRIVATE
    // USE ONLY FOR TESTING PATTERNS
    static vector<string> createPostfix(vector<string> &infix);
    static set<string> createPatterns(vector<string> &infix);

private:
    static map<string, int> precedence;
};
