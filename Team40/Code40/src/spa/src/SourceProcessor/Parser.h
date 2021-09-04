#pragma once
#include "../common/model/Procedure.h"
#include "../common/model/Program.h"
#include "../common/model/statement/Statement.h"
#include "Line.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Parser {
public:
    // parse file input
    vector<Line> parseFile(fstream &file);
    vector<string> parseLine(string input);
    string clean(string input);

    // parse preprocessed file
    Program parseProgram(vector<Line> programLines);
    Procedure parseProcedure(vector<string> content);
    Statement parseStatement(vector<string> content, int index);
    Statement parseReadStatement(vector<string> content, int index);
    Statement parsePrintStatement(vector<string> content, int index);

    // special keywords
    bool isProc(vector<string> inputLine);
    bool isReadStmt(vector<string> inputLine);
    bool isPrintStmt(vector<string> inputLine);
    bool isCallStmt(vector<string> inputLine);
    bool isWhileStmt(vector<string> inputLine);
    bool isIfStmt(vector<string> inputLine);
    bool isKeyword(string input);

    // special characters
    bool isBracket(char input);
    bool isOperator(char input);
    bool isSemiColon(char input);
};
