#include "source_processor/Parser.h"
#include "source_processor/AssignStatementParser.h"
#include "source_processor/CallStatementParser.h"
#include "source_processor/IfStatementParser.h"
#include "source_processor/PrintStatementParser.h"
#include "source_processor/ProcedureParser.h"
#include "source_processor/ReadStatementParser.h"
#include "source_processor/StatementParser.h"
#include "source_processor/WhileStatementParser.h"
#include <algorithm>
#include <iostream>

// parse file input

vector<Line> Parser::parseFile(fstream &file) {
    vector<Line> programLines = {};
    string input;
    int stmtNum = 0;
    while (getline(file, input)) {
        vector<string> inputLine = parseLine(input);
        // The procedure definition does not receive an index.
        if (!inputLine.empty() && inputLine[0] != "}" &&
            inputLine[0] != "else" && !isProc(inputLine)) {
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
                if (i < input.size() - 1) {
                    if (!isalnum(input.at(i + 1))) {
                        addString(currString, inputLine);
                    }
                } else {
                    addString(currString, inputLine);
                }
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
        if (isProc(currContent)) {
            if (!currProc.getName().empty()) {
                program.addToProcLst(currProc);
            }
            ProcedureParser procParser(currContent);
            currProc = procParser.parseProcedure();
        } else if (!currContent.empty() && currContent[0] != "}" &&
                   currContent[0] != "else") {
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
    StatementParser stmtParser(content, index, programLines, programIndex);
    if (stmtParser.isReadStmt(content)) {
        ReadStatementParser readParser(content, index);
        return readParser.parseReadStatement();
    } else if (stmtParser.isPrintStmt(content)) {
        PrintStatementParser printParser(content, index);
        return printParser.parsePrintStatement();
    } else if (stmtParser.isCallStmt(content)) {
        CallStatementParser callParser(content, index);
        return callParser.parseCallStatement();
    } else if (stmtParser.isAssignStmt(content)) {
        AssignStatementParser assignParser(content, index);
        return assignParser.parseAssignStatement();
    } else if (stmtParser.isWhileStmt(content)) {
        WhileStatementParser whileParser(content, index, programLines,
                                         programIndex);
        return whileParser.parseWhileStatement();
    } else if (stmtParser.isIfStmt(content)) {
        IfStatementParser ifParser(content, index, programLines, programIndex);
        return ifParser.parseIfStatement();
    } else {
        return Statement(0, StatementType::NONE);
    }
}

// special keywords

bool Parser::isProc(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "procedure") !=
           inputLine.end();
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
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isdigit(c)); }) == input.end();
}

bool Parser::isName(string input) {
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}
