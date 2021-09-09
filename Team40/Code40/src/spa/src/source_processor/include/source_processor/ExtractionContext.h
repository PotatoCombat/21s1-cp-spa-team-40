#pragma once

#include <optional>
#include "pkb/Abstractions.h"

using namespace std;

class ExtractionContext {
private:
    static ExtractionContext *instance;
    optional<ProcIndex> currentProc;
    optional<StmtIndex> prevStatement;
    optional<StmtIndex> currentStatement;
    optional<StmtIndex> parentStatement;
    optional<StmtIndex> modifyingStatement;
    optional<StmtIndex> usingStatement;

    ExtractionContext() = default;

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();

    optional<ProcIndex> getCurrentProc();
    void setCurrentProc(ProcIndex procIndex);

    optional<StmtIndex> getPrevStatement();
    void setPrevStatement(StmtIndex stmtIndex);

    optional<StmtIndex> getCurrentStatement();
    void setCurrentStatement(StmtIndex stmtIndex);

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
