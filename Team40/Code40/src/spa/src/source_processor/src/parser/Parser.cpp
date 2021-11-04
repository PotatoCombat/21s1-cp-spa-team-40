#include "source_processor/parser/Parser.h"

/**
 * Parses a list of tokenized program lines by passing them to their respective parser and adding
 * the returned Entity object into the program's ast
 * @return the program's abstract syntax tree
 */
Program Parser::parseProgram(vector<Line> programLines) {
    Program program;
    Procedure *currProc = nullptr;
    for (int i = 0; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();

        if (isProc(currContent)) {
            // ... stmtLst Tokens::CHAR_CLOSE_BRACE
            if (i > 0) {
                if (programLines[i - 1].getContent()[0] != Tokens::SYMBOL_CLOSE_BRACE) {
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
    return find(content.begin(), content.end(), Tokens::SYMBOL_ASSIGN) != content.end();
}

bool Parser::isProc(vector<string> content) {
    return (content[0] == Tokens::KEYWORD_PROCEDURE) && (!isAssignStmt(content));
}

bool Parser::isStmt(vector<string> content) {
    return ((!content.empty() && content[0] != Tokens::SYMBOL_CLOSE_BRACE &&
             content[0] != Tokens::KEYWORD_ELSE) ||
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
