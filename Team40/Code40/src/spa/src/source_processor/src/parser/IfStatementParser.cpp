#include "source_processor/parser/IfStatementParser.h"
#include "source_processor/parser/ExpressionParser.h"
#include "source_processor/parser/Parser.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index, vector<Line> programLines)
    : EntityParser(content, index), programLines(programLines) {
    entity = new Statement(index, StatementType::IF);
};

Statement *IfStatementParser::parseEntity(int &programIndex) {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), Tokens::KEYWORD_IF);
    vector<string>::iterator endItr =
        find(content.begin(), content.end(), Tokens::SYMBOL_OPEN_BRACE);
    if (endItr == content.end())
        throw invalid_argument("invalid if statement");
    // if: 'if' Tokens::CHAR_OPEN_BRACKET cond_expr Tokens::CHAR_CLOSE_BRACKET 'then'
    // Tokens::CHAR_OPEN_BRACE entityLst
    // Tokens::CHAR_CLOSE_BRACE 'else' Tokens::CHAR_OPEN_BRACE entityLst
    // Tokens::CHAR_CLOSE_BRACE
    if (next(ifItr) == content.end() || prev(endItr) == content.end() ||
        prev(prev(endItr)) == content.end()) {
        throw invalid_argument("invalid if statement");
    }
    if (*next(ifItr) != Tokens::SYMBOL_OPEN_BRACKET || *prev(endItr) != Tokens::KEYWORD_THEN ||
        *prev(prev(endItr)) != Tokens::SYMBOL_CLOSE_BRACKET) {
        throw invalid_argument("invalid if statement");
    }
    if (next(next(ifItr)) == content.end()) {
        throw invalid_argument("invalid if statement");
    }
    ExpressionParser exprParser(vector<string>(next(next(ifItr)), prev(prev(endItr))), entity);
    vector<string> condLst = exprParser.parseExpression();
    entity->setExpressionLst(condLst);
    parseChildStmts(programIndex);
    if (entity->getThenStmtLst().size() == 0 || entity->getElseStmtLst().size() == 0) {
        throw invalid_argument("nested entityLst should have at least one statement.");
    }
    return entity;
}

void IfStatementParser::parseChildStmts(int &programIndex) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        programIndex = i;
        if (currContent[0] == Tokens::SYMBOL_CLOSE_BRACE) {
            terminator++;
            // ... entityLst Tokens::CHAR_CLOSE_BRACE 'else' Tokens::CHAR_OPEN_BRACEentityLst
            // Tokens::CHAR_CLOSE_BRACE
            if (terminator == 1 && programLines[i + 1].getContent()[0] != Tokens::KEYWORD_ELSE &&
                programLines[i + 1].getContent()[1] != Tokens::SYMBOL_OPEN_BRACE) {
                throw invalid_argument("invalid if statement");
            }
            if (terminator == 2) {
                break;
            }
            continue;
        }
        Parser parser;
        if (parser.isStmt(currContent)) {
            StatementParser entityParser(currContent, currIndex, programLines, i);
            Statement *nestedStmt = entityParser.parseEntity();
            if (terminator == 0) {
                entity->addThenStmt(nestedStmt);
            } else if (terminator == 1) {
                entity->addElseStmt(nestedStmt);
            }
        }
    }
    // ... entityLst Tokens::CHAR_CLOSE_BRACE 'else' Tokens::CHAR_OPEN_BRACEentityLst
    // Tokens::CHAR_CLOSE_BRACE
    if (terminator != 2) {
        throw invalid_argument("invalid if statement");
    }
}
