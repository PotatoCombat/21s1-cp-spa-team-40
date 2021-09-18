#include "source_processor/parser/Parser.h"

Program Parser::parseProgram(vector<Line> programLines, Program &program) {
    Procedure *currProc = nullptr;
    for (int i = 0; i < programLines.size(); i++) {
        int currIndex = programLines[i].getIndex();
        vector<string> currContent = programLines[i].getContent();

        if ((currContent[0] == "procedure") && (!isAssignStmt(currContent))) {
            // ... stmtLst '}'
            if (i > 0) {
                if (programLines[i - 1].getContent()[0] != "}") {
                    throw invalid_argument("invalid program");
                }
            }
            checkAndAddValidProcedure(currProc, program);
            ProcedureParser procParser(currContent);
            currProc = procParser.parseProcedure();

        } else if (!currContent.empty() && currContent[0] != "}" &&
                   currContent[0] != "else") {
            Statement *stmt =
                parseStatement(currContent, currIndex, programLines, i);
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

Statement *Parser::parseStatement(vector<string> content, int index,
                                  vector<Line> programLines,
                                  int &programIndex) {
    StatementParser stmtParser(content);
    if (stmtParser.isAssignStmt()) {
        AssignStatementParser assignParser(content, index);
        return assignParser.parseAssignStatement();
    } else if (stmtParser.isReadStmt()) {
        ReadStatementParser readParser(content, index);
        return readParser.parseReadStatement();
    } else if (stmtParser.isPrintStmt()) {
        PrintStatementParser printParser(content, index);
        return printParser.parsePrintStatement();
    } else if (stmtParser.isCallStmt()) {
        CallStatementParser callParser(content, index);
        return callParser.parseCallStatement();
    } else if (stmtParser.isWhileStmt()) {
        WhileStatementParser whileParser(content, index, programLines);
        return whileParser.parseWhileStatement(programIndex);
    } else if (stmtParser.isIfStmt()) {
        IfStatementParser ifParser(content, index, programLines);
        return ifParser.parseIfStatement(programIndex);
    } else {
        throw invalid_argument("invalid statement type");
    }
}

void Parser::checkAndAddValidProcedure(Procedure *currProc, Program &program) {
    if (currProc != nullptr) {
        // stmtLst: stmt+
        if (currProc->getStmtLst().size() == 0) {
            throw invalid_argument("procedure should have at least one "
                                   "statement.");
        }
        program.addToProcLst(currProc);
    }
}

void Parser::checkValidProgram(Program program) {
    // program: procedure+
    if (program.getProcLst().size() == 0) {
        throw invalid_argument("program should have at least one procedure.");
    }
}
