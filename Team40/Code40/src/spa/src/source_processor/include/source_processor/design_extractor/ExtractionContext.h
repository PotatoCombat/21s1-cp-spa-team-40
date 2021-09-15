#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Procedure.h"
#include "source_processor/design_extractor/EntityContext.h"
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class ExtractionContext {
private:
    optional<Procedure *> currentProcedure;
    unordered_map<ProcName, unordered_set<ProcName>> procDependencyMap;
    unordered_map<ProcName, int> procIndegreesCounter;

    static ExtractionContext *instance;
    EntityContext<Statement> parentContext;
    EntityContext<Statement> followsContext;
    optional<Statement *> usingStatement;
    optional<Statement *> modifyingStatement;

    // NOTE: Do not autofix to default constructor here
    ExtractionContext() {}

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();

    EntityContext<Statement> getFollowsContext();
    EntityContext<Statement> getParentContext();

    optional<Procedure *> getCurrentProcedure();
    void setCurrentProcedure(Procedure *procedure);
    void unsetCurrentProcedure(Procedure *procedure);
    optional<Statement *> getUsingStatement();
    void setUsingStatement(Statement *statement);
    void unsetUsingStatement(Statement *statement);
    optional<Statement *> getModifyingStatement();
    void setModifyingStatement(Statement *statement);
    void unsetModifyingStatement(Statement *statement);

    void addProcDependency(ProcName caller, ProcName callee);
    bool hasCyclicalProcDependency(ProcName caller, ProcName callee);
    unordered_set<ProcName> getProcDependencies(ProcName from);
    vector<ProcName> getTopologicallySortedProcNames();

    void resetTransientContexts();
    void reset();
};
