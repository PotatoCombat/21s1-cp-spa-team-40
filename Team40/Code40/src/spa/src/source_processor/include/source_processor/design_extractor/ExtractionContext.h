#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/model/Abstractions.h"
#include "common/model/Procedure.h"

#include "source_processor/design_extractor/EntityContext.h"

using namespace std;

class ExtractionContext {
private:
    unordered_map<ProcName, unordered_set<ProcName>> procDependencyMap;
    unordered_map<ProcName, int> procIndegreesCounter;
    unordered_map<ProcName, StmtIndex> procFirstExecutableStatements;
    unordered_map<ProcName, set<StmtIndex>> procLastExecutableStatements;

    static ExtractionContext *instance;

    optional<Procedure *> currentProcedure;

    optional<Statement *> modifyingStatement;
    optional<Statement *> usingStatement;

    vector<Statement *> parentStatements;
    vector<Statement *> precedingStatements;
    vector<Statement *> previousStatements;

    // NOTE: Do not autofix to default constructor here
    ExtractionContext() {}

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();

    optional<Procedure *> getCurrentProcedure();
    void setCurrentProcedure(Procedure *procedure);
    void unsetCurrentProcedure(Procedure *procedure);

    StmtIndex getFirstExecutableStatement(const ProcName &procName);
    set<StmtIndex> getLastExecutableStatements(const ProcName &procName);
    void setFirstExecutableStatement(const ProcName &procName,
                                     StmtIndex stmtIndex);
    void setLastExecutableStatements(const ProcName &procName,
                                     set<StmtIndex> stmtIndices);

    optional<Statement *> getModifyingStatement();
    void setModifyingStatement(Statement *statement);
    void unsetModifyingStatement(Statement *statement);
    optional<Statement *> getUsingStatement();
    void setUsingStatement(Statement *statement);
    void unsetUsingStatement(Statement *statement);

    vector<Statement *> getPrecedingStatements();
    void setPrecedingStatement(Statement *statement);
    void unsetPrecedingStatement(Statement *statement);
    void clearPrecedingStatements();
    vector<Statement *> getParentStatements();
    void setParentStatement(Statement *statement);
    void unsetParentStatement(Statement *statement);
    void clearParentStatements();
    vector<Statement *> getPreviousStatements();
    void setPreviousStatement(Statement *statement);
    void unsetPreviousStatement(Statement *statement);
    void clearPreviousStatements();

    void registerProcDependency(const ProcName &caller, const ProcName &callee);
    bool hasCyclicalProcDependency(const ProcName &caller,
                                   const ProcName &callee);
    unordered_set<ProcName> getProcDependencies(const ProcName &from);
    vector<ProcName> getTopologicallySortedProcNames();

    void resetTransientContexts();
    void reset();
};
