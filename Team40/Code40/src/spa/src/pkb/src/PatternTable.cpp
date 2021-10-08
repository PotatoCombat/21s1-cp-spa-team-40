#include "pkb/PatternTable.h"

#include <stack>
#include <tuple>

using namespace std;

PatternTable::PatternTable() = default;

void PatternTable::insertAssignPattern(Statement *stmt) {
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    vector<string> exprList = stmt->getExpressionLst();

    // ***************
    // Partial Records
    // ***************
    vector<string> postfix = createPostfix(exprList);
    vector<string> patterns = createPartialPatterns(postfix);

    set<string> uniquePatterns = set<string>(patterns.begin(), patterns.end());

    // Note
    // k : v means a map entry, where Key = k, Value = v
    for (const auto &p : uniquePatterns) {
        // Complete Record, eg. ("x", _"y * 5"_)
        Record record = make_pair(varName, p);

        // Record with wildcard varName, eg. (_, _"y * 5"_)
        Record recordWithoutVarName = make_pair(WILDCARD, p);

        // Map ("x", _"y * 5"_) : { stmt#1 }
        insertStmtWithPartialPattern(record, stmtIndex);

        // Map (_, _"y * 5"_) : { stmt#1 }
        insertStmtWithPartialPattern(recordWithoutVarName, stmtIndex);

        // Map stmt#1 : { ("x", _"y * 5"_), (_, _"y * 5"_) }
        insertPartialPatternsOfStmt(stmtIndex, {record, recordWithoutVarName});
    }

    // Record with wildcard pattern, eg. ("x", _)
    Record recordWithoutPattern = make_pair(varName, WILDCARD);

    // Map ("x", _) : { stmt#1 }
    insertStmtWithPartialPattern(recordWithoutPattern, stmtIndex);

    // Map stmt#1 : { ("x", _) }
    insertPartialPatternsOfStmt(stmtIndex, {recordWithoutPattern});

    // ***************
    // Exact Records
    // ***************
    string exactPattern = patterns.back();

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

set<StmtIndex> PatternTable::getPartialAssignPatternStmts(VarName varName, ExpressionList exprList) {
    vector<string> postfix = createPostfix(exprList);
    string pattern = createExactPattern(postfix);
    Record record = make_pair(varName, pattern);

    auto kvp = stmtsWithPartialPatternMap.find(record); // kvp stands for Key-Value Pair (map entry).
    if (kvp == stmtsWithPartialPatternMap.end()) { // Could not find a map entry with record as the key.
        return {};
    }
    return kvp->second;
}

set<StmtIndex> PatternTable::getExactAssignPatternStmts(VarName varName, ExpressionList exprList) {
    vector<string> postfix = createPostfix(exprList);
    string pattern = createExactPattern(postfix);
    Record record = make_pair(varName, pattern);

    auto kvp = stmtsWithExactPatternMap.find(record);
    if (kvp == stmtsWithExactPatternMap.end()) {
        return {};
    }
    return kvp->second;
}

bool PatternTable::partialAssignPattern(StmtIndex stmtIndex, VarName varName, ExpressionList exprList) {
    vector<string> postfix = createPostfix(exprList);
    string pattern = createExactPattern(postfix);
    Record record = make_pair(varName, pattern);

    auto kvp = partialPatternsOfStmtMap.find(stmtIndex);
    if (kvp == partialPatternsOfStmtMap.end()) {
        return false;
    }
    return kvp->second.find(record) != kvp->second.end();
}

bool PatternTable::exactAssignPattern(StmtIndex stmtIndex, VarName varName, ExpressionList exprList) {
    vector<string> postfix = createPostfix(exprList);
    string pattern = createExactPattern(postfix);
    Record record = make_pair(varName, pattern);

    auto kvp = exactPatternsOfStmtMap.find(stmtIndex);
    if (kvp == exactPatternsOfStmtMap.end()) {
        return false;
    }
    return kvp->second.find(record) != kvp->second.end();
}

void PatternTable::insertStmtWithPartialPattern(Record record, StmtIndex stmtIndex) {
    auto kvp = stmtsWithPartialPatternMap.find(record);
    if (kvp == stmtsWithPartialPatternMap.end()) {
        stmtsWithPartialPatternMap[record] = {stmtIndex};
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

void PatternTable::insertPartialPatternsOfStmt(StmtIndex stmtIndex,
                                               set<Record> record) {
    auto kvp = partialPatternsOfStmtMap.find(stmtIndex);
    if (kvp == partialPatternsOfStmtMap.end()) {
        partialPatternsOfStmtMap[stmtIndex] = record;
    } else {
        for (auto record : record) {
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

vector<string> PatternTable::tokenizePattern(Pattern &pattern) {
    vector<string> infix;

    size_t start = 0;
    size_t end = pattern.find(' ', start);

    while (end != string::npos) {
        infix.push_back(pattern.substr(start, end - start));

        start = end + 1;
        end = pattern.find(' ', start);
    }

    if (end - start > 0) {
        infix.push_back(pattern.substr(start, end - start));
    }

    return infix;
}

vector<string> PatternTable::createPostfix(ExpressionList &exprList) {
    stack<string> stack;
    stack.push("#"); // Marks empty stack

    vector<string> postfix;
    for (const string &s : exprList) {
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

string PatternTable::createExactPattern(vector<string> &postfix) {
    stack<string> stack;

    for (const string &s : postfix) {
        bool isTerm = PRECEDENCE.count(s) == 0;
        if (isTerm) {
            stack.push(s);
        } else {
            string rightTerm = stack.top();
            stack.pop();

            string leftTerm = stack.top();
            stack.pop();

            // We need a symbol (comma ,) to separate the operands.
            // If not, the following can happen:
            // 1010+ could mean 101 + 0 or 10 + 10 (soln. 10.10+)
            // xyz+ could mean x + yz or xy + z (soln. x.yz+)
            string largerTerm = string(leftTerm)
                    .append(SEPARATOR)
                    .append(rightTerm)
                    .append(s);

            stack.push(largerTerm);
        }
    }
    if (stack.empty())
    {
        return WILDCARD;
    }
    return stack.top();
}

vector<string> PatternTable::createPartialPatterns(vector<string> &postfix) {
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

            // We need a symbol (comma ,) to separate the operands.
            // If not, the following can happen:
            // 1010+ could mean 101 + 0 or 10 + 10 (soln. 10.10+)
            // xyz+ could mean x + yz or xy + z (soln. x.yz+)
            string largerTerm = string(leftTerm)
                    .append(SEPARATOR)
                    .append(rightTerm)
                    .append(s);

            patterns.push_back(largerTerm);
            stack.push(largerTerm);
        }
    }
    return patterns;
}
