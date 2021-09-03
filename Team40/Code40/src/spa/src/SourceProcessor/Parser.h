#pragma once
#include <string>
using namespace std;

class Parser {
public:
    void parse(int index, string input);
    bool isStmt(string input);
    bool isProc(string input);
};
