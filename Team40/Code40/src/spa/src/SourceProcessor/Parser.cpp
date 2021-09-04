#include "Parser.h"
#include "../common/model/Factor.cpp"
#include "../common/model/Procedure.cpp"
#include "../common/model/Program.cpp"
#include "../common/model/Variable.cpp"
#include "../common/model/statement/PrintStatement.cpp"
#include "../common/model/statement/ReadStatement.cpp"
#include "../common/model/statement/Statement.cpp"
#include "Line.cpp"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

// parse file input

vector<Line> Parser::parseFile(fstream &file) {
    vector<Line> programLines = {};
    string input;
    int stmtNum;
    while (getline(file, input)) {
        vector<string> inputLine = parseLine(input);
        if (isProc(inputLine)) {
            stmtNum = 0;
        } else {
            stmtNum++;
        }
        if (!inputLine.empty()) {
            Line curr = Line(stmtNum, inputLine);
            programLines.push_back(curr);
        }
    }
    return programLines;
}

vector<string> Parser::parseLine(string input) {
    vector<string> inputLine;
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        char curr = input.at(i);
        currString = clean(currString);

        if (isOperator(curr) || isBracket(curr) || isSemiColon(curr)) {
            if (currString.length() != 0) {
                if (!currString.empty()) {
                    inputLine.push_back(currString);
                }
                currString = "";
            }
            if (!string(1, curr).empty()) {
                inputLine.push_back(string(1, curr));
            }

        } else {
            currString.push_back(curr);
            if (isKeyword(currString)) {
                if (!currString.empty()) {
                    inputLine.push_back(currString);
                }
                currString = "";
            }
        }
    }
    return inputLine;
}

string Parser::clean(string input) {
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

// parse preprocessed file

Program Parser::parseProgram(vector<Line> programLines) {
    Program program = Program();
    Procedure currProc("");
    for (int i = 0; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currIndex == 0) {
            if (!currProc.getName().empty()) {
                program.addToProcLst(currProc);
            }
            currProc = parseProcedure(currContent);
        } else {
            Statement stmt = parseStatement(currContent, currIndex);
            currProc.addToStmtLst(stmt);
        }
    }
    program.addToProcLst(currProc);
    return program;
}

Statement Parser::parseStatement(vector<string> content, int index) {
    if (isReadStmt(content)) {
        return parseReadStatement(content, index);
    } else if (isPrintStmt(content)) {
        return parsePrintStatement(content, index);
    } else {
        return Statement(index, StatementType::ASSIGN);
    }
    // } else if (isCallStmt(content)) {
    //     return parseCallStatement(content, index);
    // } else if (isWhileStmt(content)) {
    //     return parseWhileStatement(content, index);
    // } else if (isIfStmt(content)) {
    //     return parseIfStatement(content, index);
    // } else { // assign
    //     return parseAssignStatement(content, index);
    // }
}

Procedure Parser::parseProcedure(vector<string> content) {
    vector<string>::iterator procItr =
        find(content.begin(), content.end(), "procedure");
    string proc_name = *next(procItr);
    return Procedure(proc_name);
}

Statement Parser::parseReadStatement(vector<string> content, int index) {
    vector<string>::iterator readItr =
        find(content.begin(), content.end(), "read");
    string var_name = *next(readItr);
    // Note: variable value is unknown
    return ReadStatement(index, Variable(var_name));
}

Statement Parser::parsePrintStatement(vector<string> content, int index) {
    vector<string>::iterator printItr =
        find(content.begin(), content.end(), "print");
    string var_name = *next(printItr);
    // Note: variable value is unknown
    return PrintStatement(index, Variable(var_name));
}

// special keywords

bool Parser::isProc(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "procedure") !=
           inputLine.end();
}

bool Parser::isReadStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "read") != inputLine.end();
}

bool Parser::isPrintStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "print") != inputLine.end();
}

bool Parser::isCallStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "call") != inputLine.end();
}

bool Parser::isWhileStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "while") != inputLine.end();
}

bool Parser::isIfStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "if") != inputLine.end();
}

bool Parser::isKeyword(string input) {
    return input == "read" || input == "print" || input == "call" ||
           input == "while" || input == "if" || input == "then" ||
           input == "procedure";
}

// special characters

bool Parser::isOperator(char input) { // logical, comparison, artihmetic and
                                      // assignment (they all overlap)
    return input == '&' || input == '|' || input == '!' || input == '>' ||
           input == '<' || input == '=' || input == '+' || input == '-' ||
           input == '*' || input == '/' || input == '%';
}

bool Parser::isBracket(char input) {
    return input == '(' || input == ')' || input == '{' || input == '}';
}

bool Parser::isSemiColon(char input) { return input == ';'; }
