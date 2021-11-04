#pragma once

#include "common/model/Program.h"

#include "pkb/PKB.h"

#include "source_processor/design_extractor/ExtractionContext.h"
#include <unordered_map>

class NextBipExtractor {
private:
    PKB *pkb;
    unordered_map<ProcName, Statement *> terminalStmtsMap;
    unordered_set<ProgLineIndex> visited;
    static const inline int STARTING_TERMINAL_INDEX = -2;
    int nextTerminalIndex;

    void extractProcedure(Procedure *procedure);
    void extractStatement(Statement *statement);
    void extractCallStatement(Statement *callStatement);
    void extractCallTerminalNextBip(Statement *branchInFrom,
                                    Statement *branchInTo,
                                    Statement *branchBackFrom);
    void extractNonCallStatement(Statement *statement);
    void extractNonCallTerminalNextBip(Statement *statement);
    void extractCallStatementNextBip(Statement *branchInFrom,
                                     Statement *branchInTo,
                                     Statement *branchBackFrom,
                                     Statement *branchBackTo);
    // Helper functions
    Statement *getFirstExecutableStmt(const ProcName &procName);
    optional<Statement *>
    getStatementAfterCallStatement(StmtIndex callStmtIndex);
    bool isLastExecutableStmt(Statement *statement);
    static ProcName getCurrentProcName();
    inline int getNextTerminalIndex() { return nextTerminalIndex--; }

public:
    explicit NextBipExtractor(PKB *pkb);

    void extract(Program *program);
};