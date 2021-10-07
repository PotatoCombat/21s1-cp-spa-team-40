#include "source_processor/parser/WhileStatementParser.h"
#include "source_processor/parser/ExpressionParser.h"
#include "source_processor/parser/Parser.h"
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
    vector<string> condLst(next(next(whileItr)), prev(endItr));
    if (condLst.empty()) {
        throw invalid_argument("invalid condition");
    }
    checkValidCondition(condLst);
    stmt->setExpressionLst(condLst);
    ExpressionParser exprParser;
    exprParser.parseExpression(condLst, stmt);

    parseChildStatements(programIndex);
    return stmt;
}

void WhileStatementParser::checkValidCondition(vector<string> condLst) {
    for (int i = 0; i < condLst.size(); i++) {
        string curr = condLst[i];
        if (curr == "!" || curr == "&&" || curr == "||") {
            if (i == condLst.size() - 1) {
                throw invalid_argument("logical operator must be followed by (");
            } else if (condLst[i + 1] != "(") {
                throw invalid_argument("logical operator must be followed by (");
            }
            if (curr == "&&" || curr == "||") {
                if (i == 0) {
                    throw invalid_argument("logical operator must be preceded by )");
                } else if (condLst[i - 1] != ")") {
                    throw invalid_argument("logical operator must be preceded by )");
                }
            }
        }
    }
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
        Parser parser;
        if ((currContent[0] != "}" && currContent[0] != "else") ||
            parser.isAssignStmt(currContent)) {
            auto nestedStmt = parser.parseStatement(currContent, currIndex, programLines, i);
            stmt->addThenStmt(nestedStmt);
        }
    }
    // ... stmtLst '}'
    if (terminator != 1) {
        throw invalid_argument("invalid while statement");
    }
}
