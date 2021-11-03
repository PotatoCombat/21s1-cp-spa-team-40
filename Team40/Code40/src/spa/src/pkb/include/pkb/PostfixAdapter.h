#pragma once

#include <map>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class PostfixAdapter {
public:
    explicit PostfixAdapter(const vector<string> &infix);

    set<string> createPartialPatterns();
    string createExactPattern();

private:
    inline static string SEPARATOR = " ";
    inline static string WILDCARD = "_";

    inline static string SYMBOL_STACK_EMPTY = "#";
    inline static string SYMBOL_BRACKET_OPEN = "(";
    inline static string SYMBOL_BRACKET_CLOSE = ")";

    inline static map<string, int> PRECEDENCE {
        {SYMBOL_BRACKET_OPEN, 0},
        {SYMBOL_BRACKET_CLOSE, 0},
        {"+", 1},
        {"-", 1},
        {"*", 2},
        {"/", 2},
        {"%", 2},

        // # symbol is an empty stack marker that is used in postfix method.
        // It is placed in here so that the postfix method skips this symbol.
        {SYMBOL_STACK_EMPTY, 0},
        };

    vector<string> postfix;

    void createPostfix(const vector<string> &infix);
};
