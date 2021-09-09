#include "source_processor/StatementParser.h"
#include "source_processor/AssignStatementParser.h"
#include "source_processor/CallStatementParser.h"
#include "source_processor/IfStatementParser.h"
#include "source_processor/PrintStatementParser.h"
#include "source_processor/ReadStatementParser.h"
#include "source_processor/WhileStatementParser.h"
#include <algorithm>

StatementParser::StatementParser(vector<string> content, int index,
                                 vector<Line> programLines, int programIndex)
    : content(content), index(index), programLines(programLines),
      programIndex(programIndex) {}

Statement StatementParser::parseStatement() {
    if (isReadStmt(content)) {
        ReadStatementParser readParser(content, index);
        return readParser.parseReadStatement();
    } else if (isPrintStmt(content)) {
        PrintStatementParser printParser(content, index);
        return printParser.parsePrintStatement();
    } else if (isCallStmt(content)) {
        CallStatementParser callParser(content, index);
        return callParser.parseCallStatement();
    } else if (isAssignStmt(content)) {
        AssignStatementParser assignParser(content, index);
        return assignParser.parseAssignStatement();
    } else if (isWhileStmt(content)) {
        WhileStatementParser whileParser(content, index, programLines,
                                         programIndex);
        return whileParser.parseWhileStatement();
    } else if (isIfStmt(content)) {
        IfStatementParser ifParser(content, index, programLines, programIndex);
        return ifParser.parseIfStatement();
    } else {
        return Statement(0, StatementType::NONE);
    }
}

bool StatementParser::isReadStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "read") != inputLine.end();
}

bool StatementParser::isPrintStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "print") != inputLine.end();
}

bool StatementParser::isCallStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "call") != inputLine.end();
}

bool StatementParser::isWhileStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "while") != inputLine.end();
}

bool StatementParser::isIfStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "if") != inputLine.end();
}

bool StatementParser::isAssignStmt(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "=") != inputLine.end();
}
