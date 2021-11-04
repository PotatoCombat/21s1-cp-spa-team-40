#include "source_processor/parser/StatementParser.h"
#include <algorithm>

StatementParser::StatementParser(vector<string> content, int index, vector<Line> programLines,
                                 int &programIndex)
    : EntityParser(content, index), programLines(programLines), programIndex(programIndex) {}

/**
 * Detects the type of statement that is present on the program line and passes it to the
 * appropriate parser to be parsed
 * @return Statement object of the detected type.
 */
Statement *StatementParser::parseEntity() {
    if (isAssignStmt()) {
        AssignStatementParser assignParser(content, index);
        return assignParser.parseEntity();
    } else if (isReadStmt()) {
        ReadStatementParser readParser(content, index);
        return readParser.parseEntity();
    } else if (isPrintStmt()) {
        PrintStatementParser printParser(content, index);
        return printParser.parseEntity();
    } else if (isCallStmt()) {
        CallStatementParser callParser(content, index);
        return callParser.parseEntity();
    } else if (isWhileStmt()) {
        WhileStatementParser whileParser(content, index, programLines);
        return whileParser.parseEntity(programIndex);
    } else if (isIfStmt()) {
        IfStatementParser ifParser(content, index, programLines);
        return ifParser.parseEntity(programIndex);
    } else {
        throw invalid_argument("invalid statement type");
    }
}

bool StatementParser::isReadStmt() { return content.at(0) == Tokens::KEYWORD_READ; }

bool StatementParser::isPrintStmt() { return content.at(0) == Tokens::KEYWORD_PRINT; }

bool StatementParser::isCallStmt() { return content.at(0) == Tokens::KEYWORD_CALL; }

bool StatementParser::isWhileStmt() { return content.at(0) == Tokens::KEYWORD_WHILE; }

bool StatementParser::isIfStmt() { return content.at(0) == Tokens::KEYWORD_IF; }

bool StatementParser::isAssignStmt() {
    return find(content.begin(), content.end(), Tokens::SYMBOL_ASSIGN) != content.end();
}
