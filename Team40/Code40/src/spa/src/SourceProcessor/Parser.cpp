#include "Parser.h"
#include "../common/model/ConstantValue.cpp"
#include "../common/model/Factor.cpp"
#include "../common/model/Procedure.cpp"
#include "../common/model/Program.cpp"
#include "../common/model/Variable.cpp"
#include "../common/model/expression/Expression.cpp"
#include "../common/model/expression/SingleTermExpression.cpp"
#include "../common/model/expression/SubtractTermsExpression.cpp"
#include "../common/model/expression/SumTermsExpression.cpp"
#include "../common/model/statement/AssignStatement.cpp"
#include "../common/model/statement/CallStatement.cpp"
#include "../common/model/statement/PrintStatement.cpp"
#include "../common/model/statement/ReadStatement.cpp"
#include "../common/model/statement/Statement.cpp"
#include "../common/model/term/DivideByFactorTerm.cpp"
#include "../common/model/term/ModuloByFactorTerm.cpp"
#include "../common/model/term/MultiplyByFactorTerm.cpp"
#include "../common/model/term/SingleFactorTerm.cpp"
#include "../common/model/term/Term.cpp"
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
            currString.push_back(curr);

            if (i < input.size() - 1) {
                char next = input.at(i + 1);
                if (isOperator(next)) {
                    currString.push_back(next);
                    i++;
                }
            }
            if (!currString.empty()) {
                inputLine.push_back(currString);
            }
            currString = "";

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
    this->program = Program();
    Procedure currProc("");
    for (int i = 0; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currIndex == 0) {
            if (!currProc.getName().empty()) {
                this->program.addToProcLst(currProc);
            }
            currProc = parseProcedure(currContent);
        } else {
            Statement stmt = parseStatement(currContent, currIndex);
            currProc.addToStmtLst(stmt);
        }
    }
    this->program.addToProcLst(currProc);
    return this->program;
}

// TODO: Add pointers to program design entities (procedure, variable, constant)
Statement Parser::parseStatement(vector<string> content, int index) {
    if (isReadStmt(content)) {
        return parseReadStatement(content, index);
    } else if (isPrintStmt(content)) {
        return parsePrintStatement(content, index);
    } else if (isCallStmt(content)) {
        return parseCallStatement(content, index);
    } else if (isWhileStmt(content)) {
        return parseWhileStatement(content, index);
    } else { // assign
        return parseAssignStatement(content, index);
    }
    //
    // } else if (isIfStmt(content)) {
    //     return parseIfStatement(content, index);
    // }
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
    Conditional cond;
    vector<Statement> stmtLst;
    AssignStatement stmt = AssignStatement(index, Variable(var_name), expr);
    return stmt;
}

Statement Parser::parseWhileStatement(vector<string> content, int index) {
    vector<string>::iterator whileItr =
        find(content.begin(), content.end(), "while");
    return stmt;
}

Expression Parser::parseExpression(vector<string>::iterator exprItr) {
    vector<Factor> facLst = {};
    while (!isSemiColon(*exprItr) && !isCurlyBracket(*exprItr)) {
        if (isInteger(*exprItr)) {
            facLst.push_back(ConstantValue(stoi(*exprItr)));
        } else if (isName(*exprItr)) {
            facLst.push_back(Variable(*exprItr));
        }
        if (isArtihmeticOperator(*prev(exprItr))) {
            string opr = *prev(exprItr);
            Factor second = facLst.back();
            facLst.pop_back();
            Factor first = facLst.back();
            facLst.pop_back();
            Term *term1 = new SingleFactorTerm(first);
            Term *term2 = new SingleFactorTerm(second);
            if (opr == "+") {
                Expression *expr = new SingleTermExpression(term1);
                return SumTermsExpression(expr, term2);
            } else if (opr == "-") {
                Expression *expr = new SingleTermExpression(term1);
                return SubtractTermsExpression(expr, term2);
            } else if (opr == "*") {
                Term *term = new MultiplyByFactorTerm(term1, second);
                return SingleTermExpression(term);
            } else if (opr == "/") {
                Term *term = new DivideByFactorTerm(term1, second);
                return SingleTermExpression(term);
            } else if (opr == "%") {
                Term *term = new ModuloByFactorTerm(term1, second);
                return SingleTermExpression(term);
            }
        }
        exprItr = next(exprItr);
    }
    Term *term = new SingleFactorTerm(facLst.back());
    return SingleTermExpression(term);
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

bool Parser::isNormalBracket(string input) {
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

// modifiers
void Parser::addToVarLst(Variable var) { this->varLst.push_back(var); }
void Parser::addToConstLst(ConstantValue constVal) {
    this->constLst.push_back(constVal);
}

// getters
vector<Variable> Parser::getVarLst() { return this->varLst; }
vector<ConstantValue> Parser::getConstLst() { return this->constLst; }
Program Parser::getProgram() { return this->program; }
