#include "pkb/PatternTable.h"

#include <stack>
#include <tuple>

using namespace std;

PatternTable::PatternTable() = default;

void PatternTable::insertPatternAssign(Statement *stmt) {
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    vector<string> exprList = stmt->getExpressionLst();

    // Normal Records ============
    vector<string> postfix = createPostfix(exprList);
    set<string> patterns = createPatterns(postfix);

    for (const auto &p : patterns) {
        // Record with varName, eg. ("x", "y * 5")
        Record recordWithVarName = make_pair(varName, p);

        // Record with wildcard, eg. (_, "y * 5")
        Record recordWithWildcard = make_pair(WILDCARD, p);

        insertStmtWithPattern(recordWithVarName, stmtIndex);
        insertStmtWithPattern(recordWithWildcard, stmtIndex);

        insertPatternsOfStmt(stmtIndex,
                             {recordWithVarName, recordWithWildcard});
    }

    // Exact Records ============
    string exactPattern = createExactPattern(exprList);

    Record exactRecordWithVarName = make_pair(varName, exactPattern);
    Record exactRecordWithWildcard = make_pair(WILDCARD, exactPattern);

    insertStmtWithExactPattern(exactRecordWithVarName, stmtIndex);
    insertStmtWithExactPattern(exactRecordWithWildcard, stmtIndex);

    insertExactPatternsOfStmt(
        stmtIndex, {exactRecordWithVarName, exactRecordWithWildcard});
}

set<StmtIndex> PatternTable::getAssignsMatchingPattern(VarName varName,
                                                       Pattern pattern) {
    Record record = make_pair(varName, pattern);
    auto kvp = stmtsWithPatternMap.find(
        record); // kvp stands for Key-Value Pair (map entry).
    if (kvp ==
        stmtsWithPatternMap
            .end()) { // Could not find a map entry with record as the key.
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
            postfix.emplace_back("(");
            stack.push("(");
        } else if (s == ")") {
            while (stack.top() != "(") {
                postfix.push_back(stack.top());
                stack.pop();
            }
            postfix.emplace_back(")");
            stack.pop();
        } else {
            if (PRECEDENCE.at(s) > PRECEDENCE.at(stack.top())) {
                stack.push(s);
            } else {
                while (stack.top() != "#" &&
                       PRECEDENCE.at(s) <= PRECEDENCE.at(stack.top())) {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                stack.push(s);
            }
        }
    }

    while (stack.top() != "#") {
        postfix.push_back(
            stack.top()); // store and pop until stack is not empty.
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
        } else if (s == "(") { // Apparently string comparison is fine using ==
            continue;
        } else if (s == ")") {
            string lastTerm = stack.top();
            stack.pop();

            string largerTerm = string("(").append(lastTerm).append(")");

            patterns.push_back(largerTerm);
            stack.push(largerTerm);
        } else {
            string rightTerm = stack.top();
            stack.pop();

            string leftTerm = stack.top();
            stack.pop();

            string largerTerm = string(leftTerm).append(s).append(rightTerm);

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
