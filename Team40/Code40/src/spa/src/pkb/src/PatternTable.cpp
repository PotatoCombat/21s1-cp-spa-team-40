#include "pkb/PatternTable.h"

#include <stack>
#include <tuple>

using namespace std;

PatternTable::PatternTable() = default;

void PatternTable::insertPatternAssign(Statement *stmt) {
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    vector<string> exprList = stmt->getExpressionLst();

    // ***************
    // Normal Records
    // ***************
    vector<string> postfix = createPostfix(exprList);
    set<string> patterns = createPatterns(postfix);

    set<Record> patternsOfStmt;

    // Note
    // k : v means a map entry, where Key = k, Value = v
    for (const auto &p : patterns) {
        // Complete Record, eg. ("x", _"y * 5"_)
        Record record = make_pair(varName, p);

        // Record with wildcard varName, eg. (_, _"y * 5"_)
        Record recordWithoutVarName = make_pair(WILDCARD, p);

        // Map ("x", _"y * 5"_) : { stmt#1 }
        insertStmtWithPattern(record, stmtIndex);

        // Map (_, _"y * 5"_) : { stmt#1 }
        insertStmtWithPattern(recordWithoutVarName, stmtIndex);

        // Map stmt#1 : { ("x", _"y * 5"_), (_, _"y * 5"_) }
        insertPatternsOfStmt(stmtIndex, {record, recordWithoutVarName});
    }

    // Record with wildcard pattern, eg. ("x", _)
    Record recordWithoutPattern = make_pair(varName, WILDCARD);

    // Map ("x", _) : { stmt#1 }
    insertStmtWithPattern(recordWithoutPattern, stmtIndex);

    // Map stmt#1 : { ("x", _) }
    insertPatternsOfStmt(stmtIndex, {patternsOfStmt});

    // ***************
    // Exact Records
    // ***************
    string exactPattern = createExactPattern(exprList);

    // Exact record with pattern, eg. ("x", "y * 5")
    Record exactRecord = make_pair(varName, exactPattern);

    // Exact record with wildcard varName, eg. (_, "y * 5")
    Record exactRecordWithoutVarName = make_pair(WILDCARD, exactPattern);

    // Exact record with wildcard pattern, eg. ("x", _)
    Record exactRecordWithoutPattern = make_pair(varName, WILDCARD);

    // Map ("x", "y * 5") : { stmt#1 }
    insertStmtWithExactPattern(exactRecord, stmtIndex);

    // Map (_, "y * 5") : { stmt#1 }
    insertStmtWithExactPattern(exactRecordWithoutVarName, stmtIndex);

    // Map ("x", _) : { stmt#1 }
    insertStmtWithExactPattern(exactRecordWithoutPattern, stmtIndex);

    // Map stmt#1 : { ("x", "y * 5"), (_, "y * 5"), ("x", _) }
    insertExactPatternsOfStmt(stmtIndex,{
                                             exactRecord,
                                             exactRecordWithoutVarName,
                                             exactRecordWithoutPattern
                                         });
}

set<StmtIndex> PatternTable::getAssignsMatchingPattern(VarName varName,
                                                       Pattern pattern) {
    Record record = make_pair(varName, pattern);
    auto kvp = stmtsWithPatternMap.find(record); // kvp stands for Key-Value Pair (map entry).
    if (kvp == stmtsWithPatternMap.end()) { // Could not find a map entry with record as the key.
        return {};
    }
    return kvp->second;
}

set<StmtIndex> PatternTable::getAssignsMatchingExactPattern(VarName varName,
                                                            Pattern pattern) {
    Record record = make_pair(varName, pattern);
    auto kvp = stmtsWithExactPatternMap.find(record);
    if (kvp == stmtsWithExactPatternMap.end()) {
        return {};
    }
    return kvp->second;
}

bool PatternTable::assignMatchesPattern(StmtIndex stmtIndex, VarName varName,
                                        Pattern pattern) {
    Record record = make_pair(varName, pattern);
    auto kvp = patternsOfStmtMap.find(stmtIndex);
    if (kvp == patternsOfStmtMap.end()) {
        return false;
    }
    return kvp->second.find(record) != kvp->second.end();
}

bool PatternTable::assignMatchesExactPattern(StmtIndex stmtIndex,
                                             VarName varName, Pattern pattern) {
    Record record = make_pair(varName, pattern);
    auto kvp = exactPatternsOfStmtMap.find(stmtIndex);
    if (kvp == exactPatternsOfStmtMap.end()) {
        return false;
    }
    return kvp->second.find(record) != kvp->second.end();
}

void PatternTable::insertStmtWithPattern(Record record, StmtIndex stmtIndex) {
    auto kvp = stmtsWithPatternMap.find(record);
    if (kvp == stmtsWithPatternMap.end()) {
        stmtsWithPatternMap[record] = {stmtIndex};
    } else {
        kvp->second.insert(stmtIndex);
    }
}

void PatternTable::insertStmtWithExactPattern(Record record,
                                              StmtIndex stmtIndex) {
    auto kvp = stmtsWithExactPatternMap.find(record);
    if (kvp == stmtsWithExactPatternMap.end()) {
        stmtsWithExactPatternMap[record] = {stmtIndex};
    } else {
        kvp->second.insert(stmtIndex);
    }
}

void PatternTable::insertPatternsOfStmt(StmtIndex stmtIndex,
                                        set<Record> records) {
    auto kvp = patternsOfStmtMap.find(stmtIndex);
    if (kvp == patternsOfStmtMap.end()) {
        patternsOfStmtMap[stmtIndex] = records;
    } else {
        for (auto record : records) {
            kvp->second.insert(record);
        }
    }
}

void PatternTable::insertExactPatternsOfStmt(StmtIndex stmtIndex,
                                             set<Record> records) {
    auto kvp = exactPatternsOfStmtMap.find(stmtIndex);
    if (kvp == exactPatternsOfStmtMap.end()) {
        exactPatternsOfStmtMap[stmtIndex] = records;
    } else {
        for (auto record : records) {
            kvp->second.insert(record);
        }
    }
}

vector<string> PatternTable::createPostfix(vector<string> &infix) {
    stack<string> stack;
    stack.push("#"); // Marks empty stack

    vector<string> postfix;
    for (const string &s : infix) {
        bool isTerm = PRECEDENCE.count(s) == 0;
        if (isTerm) {
            postfix.push_back(s);
        } else if (s == "(") { // Apparently string comparison is fine using ==
            stack.push("(");
        } else if (s == ")") {
            while (stack.top() != "(") {
                postfix.push_back(stack.top());
                stack.pop();
            }
            stack.pop();
        } else {
            if (PRECEDENCE.at(s) > PRECEDENCE.at(stack.top())) {
                stack.push(s);
            } else {
                while (stack.top() != "#" && PRECEDENCE.at(s) <= PRECEDENCE.at(stack.top())) {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                stack.push(s);
            }
        }
    }

    while (stack.top() != "#") {
        postfix.push_back(stack.top()); // store and pop until stack is not empty.
        stack.pop();
    }

    return postfix;
}

set<string> PatternTable::createPatterns(vector<string> &postfix) {
    stack<string> stack;
    stack.push("#"); // Marks empty stack

    vector<string> patterns;
    for (const string &s : postfix) {
        bool isTerm = PRECEDENCE.count(s) == 0;
        if (isTerm) {
            patterns.push_back(s);
            stack.push(s);
        } else {
            string rightTerm = stack.top();
            stack.pop();

            string leftTerm = stack.top();
            stack.pop();

            string largerTerm = string("(")
                                    .append(leftTerm)
                                    .append(s)
                                    .append(rightTerm)
                                    .append(")");

            patterns.push_back(largerTerm);
            stack.push(largerTerm);
        }
    }
    set<string> uniquePatterns = set<string>(patterns.begin(), patterns.end());
    return uniquePatterns;
}

string PatternTable::createExactPattern(vector<string> &exprList) {
    string exactPattern;
    for (const string &s : exprList) {
        exactPattern.append(s);
    }
    return exactPattern;
}
