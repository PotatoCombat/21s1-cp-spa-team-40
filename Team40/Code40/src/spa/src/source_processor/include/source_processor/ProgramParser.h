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

class ProgramParser {
public:
    Program parseProgram(vector<Line> programLines, Program &program) {
        Procedure *currProc = nullptr; //new Procedure("");
        for (int i = 0; i < programLines.size(); i++) {
            int currIndex = programLines[i].getIndex();
            vector<string> currContent = programLines[i].getContent();
            if (isProc(currContent)) {
                if (currProc != nullptr) {
//                if (!currProc->getName().empty()) {
                    program.addToProcLst(currProc);
                }
                ProcedureParser procParser(currContent);
                currProc = procParser.parseProcedure();
            } else if (!currContent.empty() && currContent[0] != "}" &&
                       currContent[0] != "else") {
                Statement* stmt =
                    parseStatement(currContent, currIndex, programLines, i);
                currProc->addToStmtLst(stmt);
            }
        }
        program.addToProcLst(currProc);
        return program;
    }

    Statement* parseStatement(vector<string> content, int index,
                             vector<Line> programLines, int &programIndex) {
        StatementParser stmtParser(content, index, programLines, programIndex);
        if (stmtParser.isReadStmt(content)) {
            ReadStatementParser readParser(content, index);
            return readParser.parseReadStatement();
        } else if (stmtParser.isPrintStmt(content)) {
            PrintStatementParser printParser(content, index);
            return printParser.parsePrintStatement();
        } else if (stmtParser.isCallStmt(content)) {
            CallStatementParser callParser(content, index);
            return callParser.parseCallStatement();
        } else if (stmtParser.isAssignStmt(content)) {
            AssignStatementParser assignParser(content, index);
            return assignParser.parseAssignStatement();
        } else if (stmtParser.isWhileStmt(content)) {
            WhileStatementParser whileParser(content, index, programLines,
                                             programIndex);
            return whileParser.parseWhileStatement(programIndex);
        } else if (stmtParser.isIfStmt(content)) {
            IfStatementParser ifParser(content, index, programLines,
                                       programIndex);
            return ifParser.parseIfStatement(programIndex);
        } else {
            throw runtime_error("Invalid statement!");
        }
    }

    bool isProc(vector<string> inputLine) {
        return find(inputLine.begin(), inputLine.end(), "procedure") !=
               inputLine.end();
    }
};
