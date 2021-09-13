#pragma once

#include "common/model/Procedure.h"
#include "pkb/Abstractions.h"
#include "source_processor/design_extractor/EntityContext.h"
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class ExtractionContext {
private:
    static ExtractionContext *instance;
    const EntityContext<Procedure> *procedureContext;
    const EntityContext<Statement> *parentContext;
    const EntityContext<Statement> *followsContext;
    optional<Statement *> usingStatement;
    optional<Statement *> modifyingStatement;
    unordered_map<ProcName, unordered_set<ProcName>> procDependencyTree;

    // NOTE: Do not autofix to default constructor here
    ExtractionContext() {}

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();

    static EntityContext<Procedure> &getProcedureContext();
    static EntityContext<Statement> &getFollowsContext();
    static EntityContext<Statement> &getParentContext();

    optional<Statement *> getUsingStatement();
    void setUsingStatement(Statement *statement);
    void unsetUsingStatement(Statement *statement);
    optional<Statement *> getModifyingStatement();
    void setModifyingStatement(Statement *statement);
    void unsetModifyingStatement(Statement *statement);

    void addProcDependency(ProcName from, ProcName to);
    unordered_set<ProcName> getProcDependencies(ProcName from);
};
