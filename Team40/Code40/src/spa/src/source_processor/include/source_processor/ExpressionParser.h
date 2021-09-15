#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Variable.h"
#include <common/model/Statement.h>
#include <string>
#include <vector>

using namespace std;

class ExpressionParser {
public:
    void parseExpression(vector<string> exprLst, Statement *stmt);
    bool isInteger(string input);
    bool isName(string input);
};