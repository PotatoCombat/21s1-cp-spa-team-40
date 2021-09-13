#include "source_processor/WhileStatementParser.h"
#include "source_processor/ProgramParser.h"
#include <algorithm>

WhileStatementParser::WhileStatementParser(vector<string> content, int index,
                                           vector<Line> programLines,
                                           int &programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement WhileStatementParser::parseWhileStatement(int &programIndex) {
    vector<string>::iterator whileItr =
        find(content.begin(), content.end(), "while");
    Statement stmt = Statement(index, StatementType::WHILE);
    vector<string>::iterator endItr = find(content.begin(), content.end(), "{");
    vector<string> condLst(next(next(whileItr)), prev(endItr));
    stmt.setExpressionLst(condLst);
    parseChildStatements(stmt);
    programIndex = this->programIndex;
    return stmt;
}

void WhileStatementParser::parseChildStatements(Statement &stmt) {
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currContent[0] == "}") {
            break;
        }
        if (currContent[0] != "}" && currContent[0] != "else") {
            ProgramParser parser;
            Statement nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            stmt.addThenStmt(nestedStmt);
            this->programIndex = i;
            if (nestedStmt.getStatementType() == StatementType::IF) {
                i++;
            } else if (nestedStmt.getStatementType() == StatementType::WHILE) {
                i++;
            }
        }
    }
}

void WhileStatementParser::parseExpression(vector<string> exprLst,
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
