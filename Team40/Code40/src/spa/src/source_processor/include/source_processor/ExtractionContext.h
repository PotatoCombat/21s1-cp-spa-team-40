#pragma once

#include <optional>
#include "pkb/Abstractions.h"

class ExtractionContext {
private:
    static ExtractionContext *instance;
    optional<StmtIndex> prevStatement;
    optional<StmtIndex> parentStatement;
    optional<ProcIndex> currentProc;

    ExtractionContext() = default;

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();
    optional<StmtIndex> getPrevStatement();
    optional<StmtIndex> getParentStatement();
    optional<ProcIndex> getCurrentProc();

    void setPrevStatement(StmtIndex stmtIndex);
    void setParentStatement(StmtIndex stmtIndex);
    void setCurrentProc(ProcIndex procIndex);
};
