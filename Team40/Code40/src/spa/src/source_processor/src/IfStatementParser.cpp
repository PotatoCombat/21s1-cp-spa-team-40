#include "source_processor/IfStatementParser.h"
#include "source_processor/ProgramParser.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index,
                                     vector<Line> programLines,
                                     int &programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement *IfStatementParser::parseIfStatement(int &programIndex) {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "if");
    auto stmt = new Statement(index, StatementType::IF);

    vector<string>::iterator endItr =
        find(content.begin(), content.end(), "then");
    vector<string> condLst(next(next(ifItr)), prev(endItr));
    stmt->setExpressionLst(condLst);
    parseExpression(condLst, stmt);

    parseChildStatements(stmt);
    programIndex = this->programIndex;
    return stmt;
}

void IfStatementParser::parseChildStatements(Statement *stmt) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currContent[0] == "}") {
            terminator++;
            if (terminator == 2) {
                this->programIndex = i;
                break;
            }
            continue;
        }
        if (currContent[0] != "}" && currContent[0] != "else") {
            ProgramParser parser;
            auto nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            if (terminator == 0) {
                stmt->addThenStmt(nestedStmt);
            } else if (terminator == 1) {
                stmt->addElseStmt(nestedStmt);
            }
        }
    }
}

void IfStatementParser::parseExpression(vector<string> exprLst,
                                        Statement *stmt) {
    for (int i = 0; i < exprLst.size(); i++) {
        string currString = exprLst[i];
        if (isInteger(currString)) {
            auto constant = new ConstantValue(stoi(currString));
            stmt->addExpressionConst(constant);
        } else if (isName(currString)) {
            auto variable = new Variable(currString);
            stmt->addExpressionVar(variable);
        }
    }
}
