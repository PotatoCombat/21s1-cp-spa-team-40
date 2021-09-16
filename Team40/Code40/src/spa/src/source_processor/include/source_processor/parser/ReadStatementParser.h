#pragma once
#include "common/model/Statement.h"
#include <string>
#include <vector>
using namespace std;

class ReadStatementParser {
private:
    Statement *stmt;
    vector<string> content;
    int index;

public:
    ReadStatementParser(vector<string> content, int index);
    Statement *parseReadStatement();
    bool isValidName(string input);
};
