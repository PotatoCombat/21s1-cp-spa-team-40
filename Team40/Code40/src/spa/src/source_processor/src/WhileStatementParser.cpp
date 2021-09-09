#include "source_processor/WhileStatementParser.h"
#include "common/model/statement/WhileStatement.h"
#include "source_processor/Parser.h"
#include <algorithm>

WhileStatementParser::WhileStatementParser(vector<string> content, int index,
                                           vector<Line> programLines,
                                           int programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement WhileStatementParser::parseWhileStatement() {
    vector<string>::iterator whileItr =
        find(content.begin(), content.end(), "(");
    WhileStatement stmt = WhileStatement(index);
    vector<string>::iterator endItr = find(content.begin(), content.end(), ")");
    vector<string> condLst(next(whileItr), endItr);
    stmt.setCondLst(condLst);
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currIndex == 0 || hasTerminator(currContent)) {
            break;
        } else {
            Parser parser;
            Statement nestedStmt = parser.parseStatement(
                currContent, currIndex, programLines, programIndex);
            stmt.addStatement(nestedStmt);
        }
    }
    return stmt;
}
