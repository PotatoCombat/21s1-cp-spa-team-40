#include "source_processor/WhileStatementParser.h"
#include "source_processor/ProgramParser.h"
#include <algorithm>

WhileStatementParser::WhileStatementParser(vector<string> content, int index,
                                           vector<Line> programLines,
                                           int &programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement *WhileStatementParser::parseWhileStatement(int &programIndex) {
    vector<string>::iterator whileItr =
        find(content.begin(), content.end(), "while");
    auto stmt = new Statement(index, StatementType::WHILE);

    vector<string>::iterator endItr = find(content.begin(), content.end(), "{");
    vector<string> condLst(next(next(whileItr)), prev(endItr));
    stmt->setExpressionLst(condLst);
    parseExpression(condLst, stmt);

    parseChildStatements(stmt);
    programIndex = this->programIndex;
    return stmt;
}

void WhileStatementParser::parseChildStatements(Statement *stmt) {
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currContent[0] == "}") {
            this->programIndex = i;
            break;
        }
        if (currContent[0] != "}" && currContent[0] != "else") {
            ProgramParser parser;
            auto nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            stmt->addThenStmt(nestedStmt);
        }
    }
}

void WhileStatementParser::parseExpression(vector<string> exprLst,
                                           Statement *stmt) {
    for (int i = 0; i < exprLst.size(); i++) {
        string currString = exprLst[i];
        if (isInteger(currString)) {
            auto constant = new ConstantValue(currString);
            stmt->addExpressionConst(constant);
        } else if (isName(currString)) {
            auto variable = new Variable(currString);
            stmt->addExpressionVar(variable);
        }
    }
}
