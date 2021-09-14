#pragma once

#include <map>
#include <set>
#include <vector>

#include "Iterator.h"
#include "common/model/Abstractions.h"
#include "common/model/Statement.h"

using namespace std;

class PatternTable {
public:
    PatternTable();

    void insertPatternAssign(
        Statement *stmt); // Note: This will most likely be in Design Extractor
    set<StmtIndex> getAssignsMatchingPattern(VarName varName, Pattern pattern);
    set<StmtIndex> getAssignsMatchingExactPattern(VarName varName,
                                                  Pattern pattern);
    bool assignMatchesPattern(StmtIndex stmtIndex, VarName varName,
                              Pattern pattern);
    bool assignMatchesExactPattern(StmtIndex stmtIndex, VarName varName,
                                   Pattern pattern);

    typedef tuple<VarName, Pattern> Record;

    // THESE STATIC METHODS WILL BE PRIVATE
    // USE THEM ONLY FOR TESTING PURPOSES
    static vector<string> createPostfix(vector<string> &infix);
    static set<string> createPatterns(vector<string> &postfix);

    static string createExactPattern(vector<string> &exprList);

private:
    inline static map<string, int> PRECEDENCE{
        {"(", 0},
        {")", 0},
        {"+", 1},
        {"-", 1},
        {"*", 2},
        {"/", 2},
        {"%", 2},

        // # symbol is an empty stack marker.is used in postfix method.
        // It is placed in here so that the postfix method skips this symbol.
        {"#", 0},
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
