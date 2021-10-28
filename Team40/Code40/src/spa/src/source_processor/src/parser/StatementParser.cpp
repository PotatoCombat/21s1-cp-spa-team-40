#include "source_processor/parser/StatementParser.h"
#include <algorithm>

StatementParser::StatementParser(vector<string> content, int index, vector<Line> programLines,
                                 int &programIndex)
    : EntityParser(content, index), programLines(programLines), programIndex(programIndex) {}

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

bool StatementParser::isReadStmt() { return content.at(0) == "read"; }

bool StatementParser::isPrintStmt() { return content.at(0) == "print"; }

bool StatementParser::isCallStmt() { return content.at(0) == "call"; }

bool StatementParser::isWhileStmt() { return content.at(0) == "while"; }

bool StatementParser::isIfStmt() { return content.at(0) == "if"; }

bool StatementParser::isAssignStmt() {
    return find(content.begin(), content.end(), "=") != content.end();
}
