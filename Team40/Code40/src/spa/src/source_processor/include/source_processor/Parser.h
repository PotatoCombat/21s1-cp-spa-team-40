#pragma once
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Statement.h"
#include "source_processor/AssignStatementParser.h"
#include "source_processor/CallStatementParser.h"
#include "source_processor/IfStatementParser.h"
#include "source_processor/Line.h"
#include "source_processor/PrintStatementParser.h"
#include "source_processor/ProcedureParser.h"
#include "source_processor/ReadStatementParser.h"
#include "source_processor/StatementParser.h"
#include "source_processor/WhileStatementParser.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class Parser {
public:
    Program parseProgram(vector<Line> programLines, Program &program) {
        Procedure *currProc = nullptr;
        for (int i = 0; i < programLines.size(); i++) {
            int currIndex = programLines[i].getIndex();
            vector<string> currContent = programLines[i].getContent();
            if (isProc(currContent)) {
                // ... stmtLst '}'
                if (i > 0) {
                    if (programLines[i - 1].getContent()[0] != "}") {
                        throw("invalid program");
                    }
                }
                if (currProc != nullptr) {
                    // stmtLst: stmt+
                    if (currProc->getStmtLst().size() == 0) {
                        throw("procedure should have at least one "
                              "statement.");
                    }
                    program.addToProcLst(currProc);
                }
                ProcedureParser procParser(currContent);
                currProc = procParser.parseProcedure();
            } else if (!currContent.empty() && currContent[0] != "}" &&
                       currContent[0] != "else") {
                Statement *stmt =
                    parseStatement(currContent, currIndex, programLines, i);
                currProc->addToStmtLst(stmt);
            }
        }
        if (currProc != nullptr) {
            // stmtLst: stmt+
            if (currProc->getStmtLst().size() == 0) {
                throw("procedure should have at least one statement.");
            }
            program.addToProcLst(currProc);
        }
        // program: procedure+
        if (program.getProcLst().size() == 0) {
            throw("program should have at least one procedure.");
        }
        return program;
    }

    Statement *parseStatement(vector<string> content, int index,
                              vector<Line> programLines, int &programIndex) {
        StatementParser stmtParser(content);
        if (stmtParser.isReadStmt()) {
            ReadStatementParser readParser(content, index);
            return readParser.parseReadStatement();
        } else if (stmtParser.isPrintStmt()) {
            PrintStatementParser printParser(content, index);
            return printParser.parsePrintStatement();
        } else if (stmtParser.isCallStmt()) {
            CallStatementParser callParser(content, index);
            return callParser.parseCallStatement();
        } else if (stmtParser.isAssignStmt()) {
            AssignStatementParser assignParser(content, index);
            return assignParser.parseAssignStatement();
        } else if (stmtParser.isWhileStmt()) {
            WhileStatementParser whileParser(content, index, programLines);
            return whileParser.parseWhileStatement(programIndex);
        } else if (stmtParser.isIfStmt()) {
            IfStatementParser ifParser(content, index, programLines);
            return ifParser.parseIfStatement(programIndex);
        } else {
            throw("invalid statement type");
        }
    }

    bool isProc(vector<string> inputLine) {
        return find(inputLine.begin(), inputLine.end(), "procedure") !=
               inputLine.end();
    }
};
