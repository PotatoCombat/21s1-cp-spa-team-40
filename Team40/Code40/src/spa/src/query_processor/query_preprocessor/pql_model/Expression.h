#pragma once

#include <string>

using namespace std;

// can be quoted expression (constant), underscore, underscore quoted expression (substring match constant)
class Expression {
protected:
    string expr;

public:
    Expression(string expr);
    string getExpr();
};
