#pragma once

#include "pkb/Abstractions.h"
#include <optional>
#include <vector>

using namespace std;

class ExtractionContext {
private:
    static ExtractionContext *instance;
    optional<ProcIndex> currentProc;
    optional<StmtIndex> prevStatement;
    optional<StmtIndex> currentStatement;
    vector<StmtIndex> parentStatements;
    vector<StmtIndex> modifyingStatements;
    vector<StmtIndex> usingStatements;

    ExtractionContext() = default;

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();

    optional<ProcIndex> getCurrentProc();
    void setCurrentProc(ProcIndex procIndex);
    void unsetCurrentProc();

    optional<StmtIndex> getPrevStatement();
    void setPrevStatement(StmtIndex stmtIndex);
    void unsetPrevStatement();

    optional<StmtIndex> getCurrentStatement();
    void setCurrentStatement(StmtIndex stmtIndex);
    void unsetCurrentStatement();

    optional<StmtIndex> getParentStatement();
    void setParentStatement(StmtIndex stmtIndex);
    void unsetParentStatement();

    optional<StmtIndex> getModifyingStatement();
    void setModifyingStatement(StmtIndex stmtIndex);
    void unsetModifyingStatement();

    optional<StmtIndex> getUsingStatement();
    void setUsingStatement(StmtIndex stmtIndex);
    void unsetUsingStatement();
};
