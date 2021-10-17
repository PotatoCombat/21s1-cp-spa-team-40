#include "source_processor/parser/IfStatementParser.h"
#include "source_processor/parser/ExpressionParser.h"
#include "source_processor/parser/Parser.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index, vector<Line> programLines)
    : content(content), index(index), programLines(programLines) {
    stmt = new Statement(index, StatementType::IF);
};

Statement *IfStatementParser::parseIfStatement(int &programIndex) {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "if");
    vector<string>::iterator endItr = find(content.begin(), content.end(), "{");
    if (endItr == content.end())
        throw invalid_argument("invalid if statement");
    // if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
    if (next(ifItr) == content.end() || prev(endItr) == content.end() ||
        prev(prev(endItr)) == content.end()) {
        throw invalid_argument("invalid if statement");
    }
    if (*next(ifItr) != "(" || *prev(endItr) != "then" || *prev(prev(endItr)) != ")") {
        throw invalid_argument("invalid if statement");
    }

    if (next(next(ifItr)) == content.end()) {
        throw invalid_argument("invalid if statement");
    }
    ExpressionParser exprParser(vector<string> (next(next(ifItr)), prev(prev(endItr))), stmt);
    vector<string> condLst = exprParser.parseExpression();
    stmt->setExpressionLst(condLst);
    parseChildStatements(programIndex);
    return stmt;
}

void IfStatementParser::parseChildStatements(int &programIndex) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        programIndex = i;
        if (currContent[0] == "}") {
            terminator++;
            // ... stmtLst '}' 'else' '{'stmtLst '}'
            if (terminator == 1 && programLines[i + 1].getContent()[0] != "else" &&
                programLines[i + 1].getContent()[1] != "{") {
                throw invalid_argument("invalid if statement");
            }
            if (terminator == 2) {
                break;
            }
            continue;
        }
        Parser parser;
        if (parser.isStmt(currContent)) {
            StatementParser stmtParser(currContent, currIndex, programLines, i);
            auto nestedStmt = stmtParser.parseStatement();
            if (terminator == 0) {
                stmt->addThenStmt(nestedStmt);
            } else if (terminator == 1) {
                stmt->addElseStmt(nestedStmt);
            }
        }
    }
    // ... stmtLst '}' 'else' '{'stmtLst '}'
    if (terminator != 2) {
        throw invalid_argument("invalid if statement");
    }
}
