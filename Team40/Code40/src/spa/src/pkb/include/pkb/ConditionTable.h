#pragma once

#include <map>
#include <set>
#include <vector>

#include "Iterator.h"
#include "common/model/Abstractions.h"
#include "common/model/Statement.h"

using namespace std;

class ConditionTable {
public:
    ConditionTable();

    inline static VarName WILDCARD = "_";

    void insertIfPattern(Statement *stmt);
    void insertWhilePattern(Statement *stmt);

    set<StmtIndex> getIfPatternStmts(VarName varName);
    set<StmtIndex> getWhilePatternStmts(VarName varName);

    bool ifPattern(StmtIndex stmtIndex, VarName varName);
    bool whilePattern(StmtIndex stmtIndex, VarName varName);

    typedef VarName Record;

    // THESE STATIC METHODS WILL BE PRIVATE
    // USE THEM ONLY FOR TESTING PURPOSES
    static vector<string> tokenizePattern(Pattern &pattern);
    static vector<string> createPatterns(ExpressionList &exprList);

private:
    inline static string SEPARATOR = " ";

    map<Record, set<StmtIndex>> stmtsWithIfPatternMap;
    map<Record, set<StmtIndex>> stmtsWithWhilePatternMap;

    map<StmtIndex, set<Record>> ifPatternsOfStmtMap;
    map<StmtIndex, set<Record>> whilePatternsOfStmtMap;

    void insertStmtWithIfPattern(Record record, StmtIndex stmtIndex);
    void insertStmtWithWhilePattern(Record record, StmtIndex stmtIndex);

    void insertIfPatternsOfStmt(StmtIndex stmtIndex, set<Record> record);
    void insertWhilePatternsOfStmt(StmtIndex stmtIndex, set<Record> record);

    bool isVariable(string input);
};
