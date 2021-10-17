#include "source_processor/parser/WhileStatementParser.h"
#include "source_processor/parser/ExpressionParser.h"
#include "source_processor/parser/Parser.h"
#include "source_processor/parser/StatementParser.h"
#include <algorithm>

WhileStatementParser::WhileStatementParser(vector<string> content, int index,
                                           vector<Line> programLines)
    : content(content), index(index), programLines(programLines) {
    stmt = new Statement(index, StatementType::WHILE);
};

Statement *WhileStatementParser::parseWhileStatement(int &programIndex) {
    vector<string>::iterator whileItr = find(content.begin(), content.end(), "while");
    vector<string>::iterator endItr = find(content.begin(), content.end(), "{");
    if (endItr == content.end())
        throw invalid_argument("invalid while statement");
    // while: 'while' '(' cond_expr ')' '{' stmtLst '}'
    if (next(whileItr) == content.end() || prev(endItr) == content.end()) {
        throw invalid_argument("invalid while statement");
    }
    if (*next(whileItr) != "(" || *prev(endItr) != ")") {
        throw invalid_argument("invalid while statement");
    }

    if (next(next(whileItr)) == content.end()) {
        throw invalid_argument("invalid while statement");
    }
    ExpressionParser exprParser(vector<string> (next(next(whileItr)), prev(endItr)), stmt);
    vector<string> condLst = exprParser.parseExpression();
    stmt->setExpressionLst(condLst);
    parseChildStatements(programIndex);
    return stmt;
}

void WhileStatementParser::parseChildStatements(int &programIndex) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        programIndex = i;
        if (currContent[0] == "}") {
            terminator++;
            break;
        }
        Parser parser;
        if (parser.isStmt(currContent)) {
            StatementParser stmtParser(currContent, currIndex, programLines, i);
            auto nestedStmt = stmtParser.parseStatement();
            stmt->addThenStmt(nestedStmt);
        }
    }
    // ... stmtLst '}'
    if (terminator != 1) {
        throw invalid_argument("invalid while statement");
    }
}
