//#include "source_processor/Parser.h"
#include "../include/source_processor/Parser.h"

// #include "common/model/statement/AssignStatement.h"
// #include "common/model/statement/CallStatement.h"
// #include "common/model/statement/PrintStatement.h"
// #include "common/model/statement/ReadStatement.h"

#include "../../common/include/common/model/statement/AssignStatement.h"
#include "../../common/include/common/model/statement/CallStatement.h"
#include "../../common/include/common/model/statement/PrintStatement.h"
#include "../../common/include/common/model/statement/ReadStatement.h"
#include "../../common/src/model/Factor.cpp"

#include <algorithm>
#include <iostream>
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
        // The procedure definition does not receive an index.
        if (isProc(inputLine)) {
            stmtNum = 0;
        } else if (!inputLine.empty()) {
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
    vector<string> inputLine = {};
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        char curr = input.at(i);
        currString = cleanString(currString);
        if (isOperator(curr) || isBracket(curr) || isSemiColon(curr)) {
            // push back previous string before special character
            addString(currString, inputLine);
            currString.push_back(curr);
            // check if operator has an additional character
            if (i < input.size() - 1) {
                char next = input.at(i + 1);
                if (isOperator(next) && isOperator(curr)) {
                    currString.push_back(next);
                    i++;
                }
            }
            addString(currString, inputLine);
        } else {
            currString.push_back(curr);
            if (isKeyword(currString)) {
                addString(currString, inputLine);
            }
        }
    }
    return inputLine;
}

void Parser::addString(string &input, vector<string> &inputVector) {
    if (!input.empty()) {
        inputVector.push_back(input);
    }
    input = "";
}

string Parser::cleanString(string input) {
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
            // empty lines, "else" keywords on a line, curly brackets on a line
            // do not receive an index
            Statement stmt =
                parseStatement(currContent, currIndex, programLines, i);
            currProc.addToStmtLst(stmt);
        }
    }
    program.addToProcLst(currProc);
    return program;
}

Statement Parser::parseStatement(vector<string> content, int index,
                                 vector<Line> programLines, int programIndex) {
    if (isReadStmt(content)) {
        return parseReadStatement(content, index);
    } else if (isPrintStmt(content)) {
        return parsePrintStatement(content, index);
    } else if (isCallStmt(content)) {
        return parseCallStatement(content, index);
    } else if (isAssignStmt(content)) {
        return parseAssignStatement(content, index);
    } else if (isWhileStmt(content)) {
        return parseWhileStatement(content, index, programLines, programIndex);
    } else if (isIfStmt(content)) {
        return parseIfStatement(content, index, programLines, programIndex);
    } else {
        return Statement(index, StatementType::NONE);
    }
}

Procedure Parser::parseProcedure(vector<string> content) {
    vector<string>::iterator procItr =
        find(content.begin(), content.end(), "procedure");
    string proc_name = *next(procItr);
    Procedure proc = Procedure(proc_name);
    return proc;
}

Statement Parser::parseReadStatement(vector<string> content, int index) {
    vector<string>::iterator readItr =
        find(content.begin(), content.end(), "read");
    string var_name = *next(readItr);
    ReadStatement stmt = ReadStatement(index, Variable(var_name));
    return stmt;
}

Statement Parser::parsePrintStatement(vector<string> content, int index) {
    vector<string>::iterator printItr =
        find(content.begin(), content.end(), "print");
    string var_name = *next(printItr);
    PrintStatement stmt = PrintStatement(index, Variable(var_name));
    return stmt;
}

Statement Parser::parseCallStatement(vector<string> content, int index) {
    vector<string>::iterator callItr =
        find(content.begin(), content.end(), "call");
    string proc_name = *next(callItr);
    CallStatement stmt = CallStatement(index, Procedure(proc_name));
    return stmt;
}

Statement Parser::parseAssignStatement(vector<string> content, int index) {
    vector<string>::iterator assignItr =
        find(content.begin(), content.end(), "=");
    string var_name = *prev(assignItr);
    AssignStatement stmt = AssignStatement(index, Variable(var_name));
    vector<string>::iterator endItr = find(content.begin(), content.end(), ";");
    vector<string> exprLst(next(assignItr), endItr);
    stmt.setExprLst(exprLst);
    return stmt;
}

Statement Parser::parseWhileStatement(vector<string> content, int index,
                                      vector<Line> programLines,
                                      int programIndex) {
    vector<string>::iterator whileItr =
        find(content.begin(), content.end(), "while");
    WhileStatement stmt = WhileStatement(index);
    vector<string>::iterator endItr = find(content.begin(), content.end(), ")");
    vector<string> condLst(next(whileItr), endItr);
    stmt.setCondLst(condLst);
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currIndex == 0 || hasTerminator(currContent)) {
            break;
        } else {
            Statement nestedStmt =
                parseStatement(currContent, currIndex, programLines, i);
            stmt.addStatement(nestedStmt);
        }
    }
    return stmt;
}

Statement Parser::parseIfStatement(vector<string> content, int index,
                                   vector<Line> programLines,
                                   int programIndex) {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "if");
    IfStatement stmt = IfStatement(index);
    vector<string>::iterator endItr = find(content.begin(), content.end(), ")");
    vector<string> condLst(next(ifItr), endItr);
    stmt.setCondLst(condLst);
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (hasTerminator(currContent)) {
            terminator++;
        }
        if (currIndex == 0 || terminator == 2) {
            break;
        } else if (terminator == 0) {
            Statement nestedStmt =
                parseStatement(currContent, currIndex, programLines, i);
            stmt.addThenStatement(nestedStmt);
        } else if (terminator == 1) {
            Statement nestedStmt =
                parseStatement(currContent, currIndex, programLines, i);
            stmt.addElseStatement(nestedStmt);
        }
    }
    return stmt;
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

bool Parser::isAssignStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "=") != inputLine.end();
}

bool Parser::isKeyword(string input) {
    return input == "read" || input == "print" || input == "call" ||
           input == "while" || input == "if" || input == "then" ||
           input == "procedure";
}

bool Parser::hasTerminator(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "}") != inputLine.end();
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

bool Parser::isArtihmeticOperator(string input) {
    return input == "+" || input == "-" || input == "*" || input == "/" ||
           input == "%";
}

bool Parser::isComparisonOperator(string input) {
    return input == ">" || input == ">=" || input == "<" || input == "<=" ||
           input == "==" || input == "!=";
}

bool Parser::isLogicalOperator(string input) {
    return input == "!" || input == "&&" || input == "||";
}

bool Parser::isCurlyBracket(string input) {
    return input == "{" || input == "}";
}

bool Parser::isRoundBracket(string input) {
    return input == "(" || input == ")";
}

bool Parser::isSemiColon(string input) { return input == ";"; }

bool Parser::isInteger(string input) {
    for (char &ch : input) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

bool Parser::isName(string input) {
    for (char &ch : input) {
        if (!isalnum(ch)) {
            return false;
        }
    }
    return true;
}
