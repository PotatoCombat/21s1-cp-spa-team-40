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
    set<StmtIndex> getAssignsWithPattern(VarName varName, Pattern pattern);
    set<StmtIndex> getAssignsWithExactPattern(VarName varName, Pattern pattern);
    //    bool pattern(StmtIndex stmt, VarName varName, Pattern pattern);

    typedef tuple<VarName, Pattern> Record;

    // THESE STATIC METHODS WILL BE PRIVATE
    // USE THEM ONLY FOR TESTING PURPOSES
    static vector<string> createPostfix(vector<string> &infix);
    static set<string> createPatterns(vector<string> &postfix);

    static string createExactPattern(vector<string> &exprList);

private:
    inline static map<string, int> PRECEDENCE {
        { "#", 0 },
        { "(", 0 },
        { ")", 0 },
        { "+", 1 },
        { "-", 1 },
        { "*", 2 },
        { "/", 2 },
    };

    inline static string WILDCARD = "_";

    map<Record, set<StmtIndex>> stmtsWithPatternMap;
    map<Record, set<StmtIndex>> stmtsWithExactPatternMap;

    map<StmtIndex, set<Record>> patternsOfStmtMap;
    map<StmtIndex, set<Record>> exactPatternsOfStmtMap;

    void insertStmtWithPattern(Record record, StmtIndex stmtIndex);
    void insertStmtWithExactPattern(Record record, StmtIndex stmtIndex);

    void insertPatternsOfStmt(StmtIndex stmtIndex, set<Record> record);
    void insertExactPatternsOfStmt(StmtIndex stmtIndex, set<Record> record);
};
