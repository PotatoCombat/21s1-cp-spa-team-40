#include "source_processor/IfStatementParser.h"
#include "common/model/statement/IfStatement.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index,
                                     vector<Line> programLines,
                                     int programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement IfStatementParser::parseIfStatement() {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "(");
    IfStatement stmt = IfStatement(index);
    vector<string>::iterator endItr = find(content.begin(), content.end(), ")");
    vector<string> condLst(next(ifItr), endItr);
    stmt.setCondLst(condLst);
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        StatementParser stmtParser =
            StatementParser(currContent, currIndex, programLines, i);
        Statement nestedStmt = stmtParser.parseStatement();
        if (hasTerminator(currContent)) {
            terminator++;
            continue;
        }
        if (currIndex == 0 || terminator == 2) {
            break;
        } else if (terminator == 0) {
            stmt.addThenStatement(nestedStmt);
        } else if (terminator == 1) {
            stmt.addElseStatement(nestedStmt);
        }
    }
    return stmt;
}

bool IfStatementParser::hasTerminator(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "}") != inputLine.end();
}
