#include "source_processor/WhileStatementParser.h"
#include "common/model/statement/IfStatement.h"
#include "source_processor/ProgramParser.h"
#include <algorithm>

WhileStatementParser::WhileStatementParser(vector<string> content, int index,
                                           vector<Line> programLines,
                                           int &programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement WhileStatementParser::parseWhileStatement(int &programIndex) {
    vector<string>::iterator whileItr =
        find(content.begin(), content.end(), "while");
    WhileStatement stmt = WhileStatement(index);
    vector<string>::iterator endItr = find(content.begin(), content.end(), "{");
    vector<string> condLst(next(next(whileItr)), prev(endItr));
    stmt.setCondLst(condLst);
    parseChildStatements(stmt);
    programIndex = this->programIndex;
    return stmt;
}

void WhileStatementParser::parseChildStatements(WhileStatement &stmt) {
    for (int i = programIndex + 1; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();
        if (hasTerminator(currContent)) {
            break;
        }
        if (!currContent.empty() && currContent[0] != "}" &&
            currContent[0] != "else") {
            ProgramParser parser;
            Statement nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            stmt.addStatement(&nestedStmt);
            this->programIndex = i;
            Statement *currStmt = &nestedStmt;
            if (nestedStmt.getStatementType() == StatementType::IF) {
                i++;
            } else if (nestedStmt.getStatementType() == StatementType::WHILE) {
                i++;
            }
        }
    }
}
