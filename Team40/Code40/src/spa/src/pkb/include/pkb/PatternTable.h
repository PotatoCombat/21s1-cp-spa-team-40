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

    inline static VarName WILDCARD = "_";

    void insertAssignPattern(Statement *stmt);
    set<StmtIndex> getPartialAssignPatternStmts(VarName varName, ExpressionList exprList);
    set<StmtIndex> getExactAssignPatternStmts(VarName varName, ExpressionList exprList);

    bool partialAssignPattern(StmtIndex stmtIndex, VarName varName, ExpressionList exprList);
    bool exactAssignPattern(StmtIndex stmtIndex, VarName varName, ExpressionList exprList);

    typedef tuple<VarName, Pattern> Record;

    // THESE STATIC METHODS WILL BE PRIVATE
    // USE THEM ONLY FOR TESTING PURPOSES
    static vector<string> tokenizePattern(Pattern &pattern);
    static vector<string> createPostfix(ExpressionList &exprList);

    static string createExactPattern(vector<string> &postfix);
    static vector<string> createPartialPatterns(vector<string> &postfix); // Contains duplicates

private:
    inline static map<string, int> PRECEDENCE{
        {"(", 0},
        {")", 0},
        {"+", 1},
        {"-", 1},
        {"*", 2},
        {"/", 2},
        {"%", 2},

        // # symbol is an empty stack marker that is used in postfix method.
        // It is placed in here so that the postfix method skips this symbol.
        {"#", 0},
    };

    inline static string SEPARATOR = " ";

    map<Record, set<StmtIndex>> stmtsWithPartialPatternMap;
    map<Record, set<StmtIndex>> stmtsWithExactPatternMap;

    map<StmtIndex, set<Record>> partialPatternsOfStmtMap;
    map<StmtIndex, set<Record>> exactPatternsOfStmtMap;

    void insertStmtWithPartialPattern(Record record, StmtIndex stmtIndex);
    void insertStmtWithExactPattern(Record record, StmtIndex stmtIndex);

    void insertPartialPatternsOfStmt(StmtIndex stmtIndex, set<Record> record);
    void insertExactPatternsOfStmt(StmtIndex stmtIndex, set<Record> record);
};
