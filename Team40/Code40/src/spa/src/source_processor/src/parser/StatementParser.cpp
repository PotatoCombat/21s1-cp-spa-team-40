#include "source_processor/parser/StatementParser.h"
#include <algorithm>

StatementParser::StatementParser(vector<string> content) : content(content) {}

bool StatementParser::isReadStmt() { return content.at(0) == "read"; }

bool StatementParser::isPrintStmt() { return content.at(0) == "print"; }

bool StatementParser::isCallStmt() { return content.at(0) == "call"; }

bool StatementParser::isWhileStmt() { return content.at(0) == "while"; }

bool StatementParser::isIfStmt() { return content.at(0) == "if"; }

bool StatementParser::isAssignStmt() {
    return find(content.begin(), content.end(), "=") != content.end();
}
