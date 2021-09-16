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
    unordered_map<ProcName, unordered_set<ProcName>> procDependencyMap;
    unordered_map<ProcName, int> procIndegreesCounter;

    static ExtractionContext *instance;

    optional<Procedure *> currentProcedure;

    optional<Statement *> modifyingStatement;
    optional<Statement *> usingStatement;
    vector<Statement *> parentStatements;
    vector<Statement *> precedingStatements;

    // NOTE: Do not autofix to default constructor here
    ExtractionContext() {}

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();

    optional<Procedure *> getCurrentProcedure();
    void setCurrentProcedure(Procedure *procedure);
    void unsetCurrentProcedure(Procedure *procedure);

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

    void addProcDependency(ProcName caller, ProcName callee);
    bool hasCyclicalProcDependency(ProcName caller, ProcName callee);
    unordered_set<ProcName> getProcDependencies(ProcName from);
    vector<ProcName> getTopologicallySortedProcNames();

    void resetTransientContexts();
    void reset();
};
