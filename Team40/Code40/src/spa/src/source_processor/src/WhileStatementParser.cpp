#include "source_processor/WhileStatementParser.h"
#include "source_processor/ExpressionParser.h"
#include "source_processor/Parser.h"
#include <algorithm>

WhileStatementParser::WhileStatementParser(vector<string> content, int index,
                                           vector<Line> programLines)
    : content(content), index(index), programLines(programLines) {
    stmt = new Statement(index, StatementType::WHILE);
};

Statement *WhileStatementParser::parseWhileStatement(int &programIndex) {
    vector<string>::iterator whileItr =
        find(content.begin(), content.end(), "while");
    vector<string>::iterator endItr = find(content.begin(), content.end(), "{");
    // while: 'while' '(' cond_expr ')' '{' stmtLst '}'
    if (*next(whileItr) != "(" || *prev(endItr) != ")") {
        throw("invalid while statement");
    }

    vector<string> condLst(next(next(whileItr)), prev(endItr));
    stmt->setExpressionLst(condLst);
    ExpressionParser exprParser;
    exprParser.parseExpression(condLst, stmt);

    parseChildStatements(programIndex);
    return stmt;
}

void WhileStatementParser::parseChildStatements(int &programIndex) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currContent[0] == "}") {
            terminator++;
            programIndex = i;
            break;
        }
        if (currContent[0] != "}" && currContent[0] != "else") {
            Parser parser;
            auto nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            stmt->addThenStmt(nestedStmt);
        }
    }
    // ... stmtLst '}'
    if (terminator != 1) {
        throw("invalid while statement");
    }
}
