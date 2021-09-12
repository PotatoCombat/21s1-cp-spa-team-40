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

    void insertPatternAssign(AssignStatement *stmt);

//    VarName getVarOfStmt(StmtIndex stmt);
//    set<Pattern> getPatternsOfStmt(StmtIndex stmt);
//    set<StmtIndex> getStmtsWithPattern(Pattern pattern);

//    bool pattern(StmtIndex stmt, VarName varName, Pattern pattern);

    // THESE METHODS WILL BE PRIVATE
    // USE ONLY FOR TESTING PATTERNS
    static vector<string> createPostfix(vector<string> &infix);
    static tuple<string, set<string>> createPatterns(vector<string> &postfix);

private:
    static map<string, int> precedence;

    map<StmtIndex, Pattern> exactPatternOfStmtMap;
    map<StmtIndex, set<Pattern>> patternsOfStmtMap;
    map<Pattern, set<StmtIndex>> stmtsWithPatternMap;
};
