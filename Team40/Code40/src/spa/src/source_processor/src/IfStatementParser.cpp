#include "source_processor/IfStatementParser.h"
#include "common/model/statement/IfStatement.h"
#include "source_processor/Parser.h"
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
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        Parser parser;
        Statement nestedStmt = parser.parseStatement(
            currContent, currIndex, programLines, programIndex);
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
