#include "source_processor/IfStatementParser.h"
#include "source_processor/ProgramParser.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index,
                                     vector<Line> programLines,
                                     int &programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement IfStatementParser::parseIfStatement(int &programIndex) {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "if");
    IfStatement stmt = IfStatement(index);
    vector<string>::iterator endItr =
        find(content.begin(), content.end(), "then");
    vector<string> condLst(next(next(ifItr)), prev(endItr));
    stmt.setExpressionLst(condLst);
    parseExpression(condLst, stmt);
    parseChildStatements(stmt);
    programIndex = this->programIndex;
    return stmt;
}

void IfStatementParser::parseChildStatements(IfStatement &stmt) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (hasTerminator(currContent)) {
            terminator++;
            continue;
        }
        if (terminator == 2) {
            break;
        }
        if (currContent[0] != "}" && currContent[0] != "else") {
            ProgramParser parser;
            Statement nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            if (terminator == 0) {
                stmt.addThenStatement(nestedStmt);
            } else if (terminator == 1) {
                stmt.addElseStatement(nestedStmt);
            }
            this->programIndex = i;
            if (nestedStmt.getStatementType() == StatementType::IF) {
                i++;
            } else if (nestedStmt.getStatementType() == StatementType::WHILE) {
                i++;
            }
        }
    }
}

void IfStatementParser::parseExpression(vector<string> exprLst,
                                        Statement &stmt) {
    for (int i = 0; i < exprLst.size(); i++) {
        string currString = exprLst[i];
        if (isInteger(currString)) {
            ConstantValue constVal(stoi(currString));
            stmt.addExpressionConst(constVal);
        } else if (isName(currString)) {
            Variable var(currString);
            stmt.addExpressionVar(var);
        }
    }
}
