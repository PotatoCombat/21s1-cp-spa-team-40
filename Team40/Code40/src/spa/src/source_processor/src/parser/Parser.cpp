#include "source_processor/parser/Parser.h"

Program Parser::parseProgram(vector<Line> programLines) {
    Program program;
    Procedure *currProc = nullptr;
    for (int i = 0; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();

        if (isProc(currContent)) {
            // ... stmtLst '}'
            if (i > 0) {
                if (programLines[i - 1].getContent()[0] != "}") {
                    throw invalid_argument("invalid program");
                }
            }
            checkAndAddValidProcedure(currProc, program);
            ProcedureParser procParser(currContent);
            currProc = procParser.parseEntity();

        } else if (isStmt(currContent)) {
            StatementParser stmtParser(currContent, currIndex, programLines, i);
            Statement *stmt = stmtParser.parseEntity();
            currProc->addToStmtLst(stmt);
        }
    }

    checkAndAddValidProcedure(currProc, program);
    checkValidProgram(program);
    return program;
}

bool Parser::isAssignStmt(vector<string> content) {
    return find(content.begin(), content.end(), "=") != content.end();
}

bool Parser::isProc(vector<string> content) {
    return (content[0] == "procedure") && (!isAssignStmt(content));
}

bool Parser::isStmt(vector<string> content) {
    return ((!content.empty() && content[0] != "}" && content[0] != "else") ||
            isAssignStmt(content));
}

void Parser::checkAndAddValidProcedure(Procedure *currProc, Program &program) {
    if (currProc != nullptr) {
        // stmtLst: stmt+
        if (currProc->getStmtLst().empty()) {
            throw invalid_argument("procedure should have at least one "
                                   "statement.");
        }
        program.addToProcLst(currProc);
    }
}

void Parser::checkValidProgram(Program program) {
    // program: procedure+
    if (program.getProcLst().empty()) {
        throw invalid_argument("program should have at least one procedure.");
    }
}
