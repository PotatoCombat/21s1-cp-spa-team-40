#include "source_processor/IfStatementParser.h"
#include "common/model/statement/WhileStatement.h"
#include "source_processor/ProgramParser.h"
#include <algorithm>

IfStatementParser::IfStatementParser(vector<string> content, int index,
                                     vector<Line> programLines,
                                     int &programIndex)
    : StatementParser(content, index, programLines, programIndex){};

Statement IfStatementParser::parseIfStatement(int &programIndex) {
    vector<string>::iterator ifItr = find(content.begin(), content.end(), "if");
    IfStatement stmt = IfStatement(index);
    vector<string>::iterator endItr =
        find(content.begin(), content.end(), "then");
    vector<string> condLst(next(next(ifItr)), prev(endItr));
    stmt.setCondLst(condLst);
    parseChildStatements(stmt);
    programIndex = this->programIndex;
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
        if (terminator == 2) {
            break;
        }
        if (currContent[0] != "}" && currContent[0] != "else") {
            ProgramParser parser;
            Statement nestedStmt =
                parser.parseStatement(currContent, currIndex, programLines, i);
            if (terminator == 0) {
                stmt.addThenStatement(nestedStmt);
            } else if (terminator == 1) {
                stmt.addElseStatement(nestedStmt);
            }
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
