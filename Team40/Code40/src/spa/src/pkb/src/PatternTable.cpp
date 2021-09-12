#include "pkb/PatternTable.h"

#include <stack>
#include <tuple>

using namespace std;

PatternTable::PatternTable() = default;

void PatternTable::insertPatternAssign(AssignStatement* stmt) {
    vector<string> exprList = stmt->getExprList();
    vector<string> postfix = createPostfix(exprList);
    tuple<string, set<string>> patterns = createPatterns(postfix);

    string exactPattern = get<0>(patterns);
    set<string> uniquePatterns = get<1>(patterns);

    exactPatternOfStmtMap[stmt->getIndex()] = exactPattern;
    patternsOfStmtMap[stmt->getIndex()] = uniquePatterns;

    for (const auto& p : uniquePatterns) {
        auto kvp = stmtsWithPatternMap.find(p);
        if (kvp == stmtsWithPatternMap.end()) {
            stmtsWithPatternMap[p] = { stmt->getIndex() };
        }
        else {
            stmtsWithPatternMap[p].insert(stmt->getIndex());
        }
    }
}

//set<StmtIndex> PatternTable::getStmtsWithPattern(VarName varName, string pattern) {
//    set<StmtIndex> s;
//    return s;
//}
//
//bool PatternTable::pattern(StmtIndex stmt, VarName varName, string pattern) {
//    return false;
//}

map<string, int> PatternTable::precedence = {
    { "#", 0 },
    { "(", 0 },
    { ")", 0 },
    { "+", 1 },
    { "-", 1 },
    { "*", 2 },
    { "/", 2 },
    };

vector<string> PatternTable::createPostfix(vector<string> &infix) {
    stack<string> stack;
    stack.push("#"); // Marks empty stack

    vector<string> postfix;
    for(const string& s : infix) {
        bool isTerm = precedence.count(s) == 0;
        if (isTerm) {
            postfix.push_back(s);
        }
        else if (s == "(") { // Apparently string comparison is fine using ==
            postfix.emplace_back("(");
            stack.push("(");
        }
        else if (s == ")") {
            while(stack.top() != "(") {
                postfix.push_back(stack.top());
                stack.pop();
            }
            postfix.emplace_back(")");
            stack.pop();
        }
        else {
            if (precedence.at(s) > precedence.at(stack.top())) {
                stack.push(s);
            }
            else {
                while (stack.top() != "#" && precedence.at(s) <= precedence.at(stack.top())) {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                stack.push(s);
            }
        }
    }

    while(stack.top() != "#") {
        postfix.push_back(stack.top());        //store and pop until stack is not empty.
        stack.pop();
    }

    return postfix;
}

tuple<string, set<string>> PatternTable::createPatterns(vector<string> &postfix) {
    stack<string> stack;
    stack.push("#"); // Marks empty stack

    vector<string> patterns;
    for(const string& s : postfix) {
        bool isTerm = precedence.count(s) == 0;
        if (isTerm) {
            patterns.push_back(s);
            stack.push(s);
        }
        else if (s == "(") { // Apparently string comparison is fine using ==
            continue;
        }
        else if (s == ")") {
            string lastTerm = stack.top();
            stack.pop();

            string largerTerm = string("(").append(lastTerm).append(")");

            patterns.push_back(largerTerm);
            stack.push(largerTerm);
        }
        else {
            string rightTerm = stack.top();
            stack.pop();

            string leftTerm = stack.top();
            stack.pop();

            string largerTerm = string(leftTerm).append(s).append(rightTerm);

            patterns.push_back(largerTerm);
            stack.push(largerTerm);
        }
    }
    string exactPattern = stack.top();
    set<string> uniquePatterns = set<string>(patterns.begin(), patterns.end());

    return make_pair(exactPattern, uniquePatterns);
}
