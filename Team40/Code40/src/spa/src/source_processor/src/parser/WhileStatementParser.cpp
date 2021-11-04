#include "source_processor/parser/WhileStatementParser.h"
#include "source_processor/parser/ExpressionParser.h"
#include "source_processor/parser/Parser.h"
#include "source_processor/parser/StatementParser.h"
#include <algorithm>

WhileStatementParser::WhileStatementParser(vector<string> content, int index,
                                           vector<Line> programLines)
    : EntityParser(content, index), programLines(programLines) {
    entity = new Statement(index, StatementType::WHILE);
};

/**
 * Parses a tokenized string identified to be a while statement into a
 * Statement object of type WHILE.
 * @return Statement object of type WHILE.
 */
Statement *WhileStatementParser::parseEntity(int &programIndex) {
    vector<string>::iterator whileItr = find(content.begin(), content.end(), Tokens::KEYWORD_WHILE);
    vector<string>::iterator endItr =
        find(content.begin(), content.end(), Tokens::SYMBOL_OPEN_BRACE);
    if (endItr == content.end())
        throw invalid_argument("invalid while statement");
    // while: 'while' '(' cond_expr ')' '{' stmtLst '}'
    if (next(whileItr) == content.end() || prev(endItr) == content.end()) {
        throw invalid_argument("invalid while statement");
    }
    if (*next(whileItr) != Tokens::SYMBOL_OPEN_BRACKET ||
        *prev(endItr) != Tokens::SYMBOL_CLOSE_BRACKET) {
        throw invalid_argument("invalid while statement");
    }
    if (next(next(whileItr)) == content.end()) {
        throw invalid_argument("invalid while statement");
    }
    ExpressionParser exprParser(vector<string>(next(next(whileItr)), prev(endItr)), entity);
    vector<string> condLst = exprParser.parseExpression();
    entity->setExpressionLst(condLst);
    parseChildStmts(programIndex);
    if (entity->getThenStmtLst().size() == 0) {
        throw invalid_argument("nested stmtLst should have at least one statement.");
    }
    return entity;
}

/**
 * Iterates through the subsequent tokenized program lines after the initial While statement
 * declaration, calls StatementParser to parse them and adds them to the stmtLst
 * The loop is terminated when the first unnested closing brace (terminator) is detected
 */
void WhileStatementParser::parseChildStmts(int &programIndex) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        programIndex = i;
        if (currContent[0] == Tokens::SYMBOL_CLOSE_BRACE) {
            terminator++;
            break;
        }
        Parser parser;
        if (parser.isStmt(currContent)) {
            StatementParser stmtParser(currContent, currIndex, programLines, i);
            Statement *nestedStmt = stmtParser.parseEntity();
            entity->addThenStmt(nestedStmt);
        }
    }
    // ... stmtLst '}'
    if (terminator != 1) {
        throw invalid_argument("invalid while statement");
    }
}
