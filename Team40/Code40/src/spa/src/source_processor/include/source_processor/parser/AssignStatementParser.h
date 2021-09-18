#pragma once
#include "common/model/Statement.h"
#include <string>
#include <vector>
using namespace std;

class AssignStatementParser {
private:
    Statement *stmt;
    vector<string> content;
    int index;

public:
    AssignStatementParser(vector<string> content, int index);
    Statement *parseAssignStatement();
    bool isValidName(string input);
};
