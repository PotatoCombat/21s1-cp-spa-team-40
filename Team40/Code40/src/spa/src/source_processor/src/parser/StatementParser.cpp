#include "source_processor/parser/StatementParser.h"
#include <algorithm>

StatementParser::StatementParser(vector<string> content, int index,
                                 vector<Line> programLines, int &programIndex)
    : content(content), index(index), programLines(programLines),
      programIndex(programIndex) {}

Statement *StatementParser::parseStatement() {
    if (isAssignStmt()) {
        AssignStatementParser assignParser(content, index);
        return assignParser.parseAssignStatement();
    } else if (isReadStmt()) {
        ReadStatementParser readParser(content, index);
        return readParser.parseReadStatement();
    } else if (isPrintStmt()) {
        PrintStatementParser printParser(content, index);
        return printParser.parsePrintStatement();
    } else if (isCallStmt()) {
        CallStatementParser callParser(content, index);
        return callParser.parseCallStatement();
    } else if (isWhileStmt()) {
        WhileStatementParser whileParser(content, index, programLines);
        return whileParser.parseWhileStatement(programIndex);
    } else if (isIfStmt()) {
        IfStatementParser ifParser(content, index, programLines);
        return ifParser.parseIfStatement(programIndex);
    } else {
        throw invalid_argument("invalid statement type");
    }
}

bool StatementParser::isReadStmt() { return content[0] == "read"; }

bool StatementParser::isPrintStmt() { return content[0] == "print"; }

bool StatementParser::isCallStmt() { return content[0] == "call"; }

bool StatementParser::isWhileStmt() { return content[0] == "while"; }

bool StatementParser::isIfStmt() { return content[0] == "if"; }

bool StatementParser::isAssignStmt() {
    return find(content.begin(), content.end(), "=") != content.end();
}
