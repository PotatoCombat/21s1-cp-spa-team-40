#include "source_processor/IfStatementParser.h"
#include "common/model/Statement.h"
//#include "source_processor/ExpressionParser.h"
#include "source_processor/ProgramParser.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index,
                                     vector<Line> programLines)
    : content(content), index(index), programLines(programLines) {
    stmt = new Statement(index, StatementType::IF);
};

Statement *IfStatementParser::parseIfStatement(int &programIndex) {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "if");
    vector<string>::iterator endItr =
        find(content.begin(), content.end(), "then");

    vector<string> condLst(next(next(ifItr)), prev(endItr));
    stmt->setExpressionLst(condLst);
    // ExpressionParser exprParser;
    // exprParser.parseExpression(condLst, stmt);

    parseChildStatements(programIndex);
    return stmt;
}

void IfStatementParser::parseChildStatements(int &programIndex) {
    int terminator = 0;
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (currContent[0] == "}") {
            terminator++;
            if (terminator == 2) {
                programIndex = i;
                break;
            }
            continue;
        }
        if (currContent[0] != "}" && currContent[0] != "else") {
            ProgramParser parser;
            auto nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            if (terminator == 0) {
                stmt->addThenStmt(nestedStmt);
            } else if (terminator == 1) {
                stmt->addElseStmt(nestedStmt);
            }
        }
    }
}
