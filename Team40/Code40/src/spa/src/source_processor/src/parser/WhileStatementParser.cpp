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

Statement *WhileStatementParser::parseEntity(int &programIndex) {
    vector<string>::iterator whileItr = find(content.begin(), content.end(), Tokens::KEYWORD_WHILE);
    vector<string>::iterator endItr =
        find(content.begin(), content.end(), Tokens::SYMBOL_OPEN_BRACE);
    if (endItr == content.end())
        throw invalid_argument("invalid while statement");
    // while: 'while' Tokens::CHAR_OPEN_BRACKET cond_expr Tokens::CHAR_CLOSE_BRACKET
    // Tokens::CHAR_OPEN_BRACE entityLst Tokens::CHAR_CLOSE_BRACE
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
        throw invalid_argument("nested entityLst should have at least one statement.");
    }
    return entity;
}

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
            StatementParser entityParser(currContent, currIndex, programLines, i);
            Statement *nestedStmt = entityParser.parseEntity();
            entity->addThenStmt(nestedStmt);
        }
    }
    // ... entityLst Tokens::CHAR_CLOSE_BRACE
    if (terminator != 1) {
        throw invalid_argument("invalid while statement");
    }
}
