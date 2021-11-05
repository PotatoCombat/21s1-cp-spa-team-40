#include "pkb/PostfixAdapter.h"

PostfixAdapter::PostfixAdapter(const vector<string> &infix) {
    createPostfix(infix);
}

void PostfixAdapter::createPostfix(const vector<string> &infix) {
    stack<string> stack;
    stack.push(SYMBOL_STACK_EMPTY); // Marks empty stack

    for (const string &s : infix) {
        bool isTerm = PRECEDENCE.count(s) == 0;
        if (isTerm) {
            postfix.push_back(s);
        } else if (s == SYMBOL_BRACKET_OPEN) { // Apparently string comparison is fine using ==
            stack.push(SYMBOL_BRACKET_OPEN);
        } else if (s == SYMBOL_BRACKET_CLOSE) {
            while (stack.top() != SYMBOL_BRACKET_OPEN) {
                postfix.push_back(stack.top());
                stack.pop();
            }
            stack.pop();
        } else {
            if (PRECEDENCE.at(s) > PRECEDENCE.at(stack.top())) {
                stack.push(s);
            } else {
                while (stack.top() != SYMBOL_STACK_EMPTY
                        && PRECEDENCE.at(s) <= PRECEDENCE.at(stack.top())) {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                stack.push(s);
            }
        }
    }

    while (stack.top() != SYMBOL_STACK_EMPTY) {
        postfix.push_back(stack.top()); // store and pop until stack is not empty.
        stack.pop();
    }
}

set<string> PostfixAdapter::createPartialPatterns() {
    stack<string> stack;
    set<string> patterns;

    for (const string &s : postfix) {
        bool isTerm = PRECEDENCE.count(s) == 0;
        if (isTerm) {
            patterns.insert(s);
            stack.push(s);
        } else {
            string rightTerm = stack.top();
            stack.pop();

            string leftTerm = stack.top();
            stack.pop();

            // We need a symbol (comma ,) to separate the operands.
            // If not, the following can happen:
            // 1010+ could mean 101 + 0 or 10 + 10 (soln. 10 10+)
            // xyz+ could mean x + yz or xy + z (soln. x yz+)
            string largerTerm = string(leftTerm)
                    .append(SEPARATOR)
                    .append(rightTerm)
                    .append(s);

            patterns.insert(largerTerm);
            stack.push(largerTerm);
        }
    }
    return patterns;
}

string PostfixAdapter::createExactPattern() {
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
            // 1010+ could mean 101 + 0 or 10 + 10 (soln. 10 10+)
            // xyz+ could mean x + yz or xy + z (soln. x yz+)
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
