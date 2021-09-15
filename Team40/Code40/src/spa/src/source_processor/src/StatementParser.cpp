#include "source_processor/StatementParser.h"
#include <algorithm>

StatementParser::StatementParser(vector<string> content) : content(content) {}

bool StatementParser::isReadStmt() { return content[0] == "read"; }

bool StatementParser::isPrintStmt() { return content[0] == "print"; }

bool StatementParser::isCallStmt() { return content[0] == "call"; }

bool StatementParser::isWhileStmt() { return content[0] == "while"; }

bool StatementParser::isIfStmt() { return content[0] == "if"; }

bool StatementParser::isAssignStmt() {
    return find(content.begin(), content.end(), "=") != content.end();
}
