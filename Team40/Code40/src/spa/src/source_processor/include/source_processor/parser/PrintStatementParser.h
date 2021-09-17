#pragma once
#include "common/model/Statement.h"
#include <string>
#include <vector>
using namespace std;

class PrintStatementParser {
private:
    Statement *stmt;
    vector<string> content;
    int index;

public:
    PrintStatementParser(vector<string> content, int index);
    Statement *parsePrintStatement();
    bool isValidName(string input);
};
