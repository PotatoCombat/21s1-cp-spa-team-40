#include "source_processor/Parser.h"
#include "source_processor/StatementParser.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

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
            currProc = parseProcedure(currContent);
        } else if (!currContent.empty() && currContent[0] != "}" &&
                   currContent[0] != "else") {
            StatementParser stmtParser =
                StatementParser(currContent, currIndex, programLines, i);
            Statement stmt = stmtParser.parseStatement();
            currProc.addToStmtLst(stmt);
        }
    }
    program.addToProcLst(currProc);
    return program;
}

Procedure Parser::parseProcedure(vector<string> content) {
    vector<string>::iterator procItr =
        find(content.begin(), content.end(), "procedure");
    string proc_name = *next(procItr);
    Procedure proc = Procedure(proc_name);
    return proc;
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
