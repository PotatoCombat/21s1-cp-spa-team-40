#include "source_processor/IfStatementParser.h"
#include "source_processor/ProgramParser.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index,
                                     vector<Line> programLines,
                                     int programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement IfStatementParser::parseIfStatement() {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "if");
    IfStatement stmt = IfStatement(index);
    vector<string>::iterator endItr =
        find(content.begin(), content.end(), "then");
    vector<string> condLst(next(next(ifItr)), prev(endItr));
    stmt.setCondLst(condLst);
    parseChildStatements(stmt);
    return stmt;
}

void IfStatementParser::parseChildStatements(IfStatement &stmt) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (hasTerminator(currContent)) {
            terminator++;
            continue;
        }
        ProgramParser parser;
        Statement nestedStmt = parser.parseStatement(
            currContent, currIndex, programLines, programIndex);
        if (currIndex == 0 || terminator == 2) {
            break;
        } else if (terminator == 0) {
            stmt.addThenStatement(nestedStmt);
        } else if (terminator == 1) {
            stmt.addElseStatement(nestedStmt);
        }
    }
}
