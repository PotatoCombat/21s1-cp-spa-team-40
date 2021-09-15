#pragma once
#include "common/model/Statement.h"
#include <string>
#include <vector>
using namespace std;

class CallStatementParser {
private:
    Statement *stmt;
    vector<string> content;
    int index;

public:
    CallStatementParser(vector<string> content, int index);
    Statement *parseCallStatement();
};
