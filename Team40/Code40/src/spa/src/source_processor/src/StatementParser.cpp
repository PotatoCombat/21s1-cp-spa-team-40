#include "source_processor/StatementParser.h"
#include <algorithm>

StatementParser::StatementParser(vector<string> content, int index,
                                 vector<Line> programLines, int programIndex)
    : content(content), index(index), programLines(programLines),
      programIndex(programIndex) {}

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

bool StatementParser::hasTerminator(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "}") != inputLine.end();
}

bool StatementParser::isInteger(string input) {
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isdigit(c)); }) == input.end();
}

bool StatementParser::isName(string input) {
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}
