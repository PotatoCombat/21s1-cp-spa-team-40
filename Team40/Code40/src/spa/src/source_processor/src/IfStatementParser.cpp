#include "source_processor/IfStatementParser.h"
#include "source_processor/ExpressionParser.h"
#include "source_processor/Parser.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index,
                                     vector<Line> programLines)
    : content(content), index(index), programLines(programLines) {
    stmt = new Statement(index, StatementType::IF);
};

Statement *IfStatementParser::parseIfStatement(int &programIndex) {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "if");
    vector<string>::iterator endItr =
        find(content.begin(), content.end(), "then");
    // if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
    if (*next(ifItr) != "(" || *prev(endItr) != ")" || *next(endItr) != "{") {
        throw("invalid if statement");
    }

    vector<string> condLst(next(next(ifItr)), prev(endItr));
    stmt->setExpressionLst(condLst);
    ExpressionParser exprParser;
    exprParser.parseExpression(condLst, stmt);

    parseChildStatements(programIndex);
    return stmt;
}

void IfStatementParser::parseChildStatements(int &programIndex) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currContent[0] == "}") {
            terminator++;
            // ... stmtLst '}' 'else' '{'stmtLst '}'
            if (terminator == 1 &&
                programLines[i + 1].getContent()[0] != "else" &&
                programLines[i + 1].getContent()[1] != "{") {
                throw("invalid if statement");
            }
            if (terminator == 2) {
                programIndex = i;
                break;
            }
            continue;
        }
        if (currContent[0] != "}" && currContent[0] != "else") {
            Parser parser;
            auto nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            if (terminator == 0) {
                stmt->addThenStmt(nestedStmt);
            } else if (terminator == 1) {
                stmt->addElseStmt(nestedStmt);
            }
        }
    }
    // ... stmtLst '}' 'else' '{'stmtLst '}'
    if (terminator != 2) {
        throw("invalid if statement");
    }
}
