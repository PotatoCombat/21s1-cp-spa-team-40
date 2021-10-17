#include "pkb/ConditionTable.h"

#include <cctype>
#include <stack>
#include <tuple>

using namespace std;

ConditionTable::ConditionTable() = default;

void ConditionTable::insertIfPattern(Statement *stmt) {
    StmtIndex stmtIndex = stmt->getIndex();
    vector<string> exprList = stmt->getExpressionLst();

    for (const string &s : exprList) {
        if (isVariable(s)) {
            insertStmtWithIfPattern(s, stmtIndex);
            insertIfPatternsOfStmt(stmtIndex, { s });
        }
    }
}

void ConditionTable::insertWhilePattern(Statement *stmt) {
    StmtIndex stmtIndex = stmt->getIndex();
    vector<string> exprList = stmt->getExpressionLst();

    for (const string &s : exprList) {
        if (isVariable(s)) {
            insertStmtWithWhilePattern(s, stmtIndex);
            insertWhilePatternsOfStmt(stmtIndex, { s });
        }
    }
}

set<StmtIndex> ConditionTable::getIfPatternStmts(VarName varName) {
    auto kvp = stmtsWithIfPatternMap.find(varName); // kvp stands for Key-Value Pair (map entry).
    if (kvp == stmtsWithIfPatternMap.end()) { // Could not find a map entry with record as the key.
        return {};
    }
    return kvp->second;
}

set<StmtIndex> ConditionTable::getWhilePatternStmts(VarName varName) {
    auto kvp = stmtsWithWhilePatternMap.find(varName); // kvp stands for Key-Value Pair (map entry).
    if (kvp == stmtsWithWhilePatternMap.end()) { // Could not find a map entry with record as the key.
        return {};
    }
    return kvp->second;
}

bool ConditionTable::ifPattern(StmtIndex stmtIndex, VarName varName) {
    auto kvp = ifPatternsOfStmtMap.find(stmtIndex);
    if (kvp == ifPatternsOfStmtMap.end()) {
        return false;
    }
    return kvp->second.find(varName) != kvp->second.end();
}

bool ConditionTable::whilePattern(StmtIndex stmtIndex, VarName varName) {
    auto kvp = whilePatternsOfStmtMap.find(stmtIndex);
    if (kvp == whilePatternsOfStmtMap.end()) {
        return false;
    }
    return kvp->second.find(varName) != kvp->second.end();
}

void ConditionTable::insertStmtWithIfPattern(Record record, StmtIndex stmtIndex) {
    auto kvp = stmtsWithIfPatternMap.find(record);
    if (kvp == stmtsWithIfPatternMap.end()) {
        stmtsWithIfPatternMap[record] = {stmtIndex};
    } else {
        kvp->second.insert(stmtIndex);
    }
}

void ConditionTable::insertStmtWithWhilePattern(Record record, StmtIndex stmtIndex) {
    auto kvp = stmtsWithWhilePatternMap.find(record);
    if (kvp == stmtsWithWhilePatternMap.end()) {
        stmtsWithWhilePatternMap[record] = {stmtIndex};
    } else {
        kvp->second.insert(stmtIndex);
    }
}

void ConditionTable::insertIfPatternsOfStmt(StmtIndex stmtIndex, set<Record> records) {
    auto kvp = ifPatternsOfStmtMap.find(stmtIndex);
    if (kvp == ifPatternsOfStmtMap.end()) {
        ifPatternsOfStmtMap[stmtIndex] = records;
    } else {
        for (auto record : records) {
            kvp->second.insert(record);
        }
    }
}

void ConditionTable::insertWhilePatternsOfStmt(StmtIndex stmtIndex, set<Record> records) {
    auto kvp = whilePatternsOfStmtMap.find(stmtIndex);
    if (kvp == whilePatternsOfStmtMap.end()) {
        whilePatternsOfStmtMap[stmtIndex] = records;
    } else {
        for (auto record : records) {
            kvp->second.insert(record);
        }
    }
}

bool ConditionTable::isVariable(string input) {
    return !input.empty() && isalpha(input.at(0));
}
