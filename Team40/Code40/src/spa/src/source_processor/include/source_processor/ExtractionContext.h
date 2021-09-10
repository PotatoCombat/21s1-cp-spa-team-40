#pragma once

#include "common/model/Procedure.h"
#include "pkb/Abstractions.h"
#include "source_processor/EntityContext.h"
#include <optional>
#include <vector>

using namespace std;

class ExtractionContext {
private:
    static ExtractionContext *instance;
    const EntityContext<Procedure> procedureContext;
    const EntityContext<Statement> parentContext;
    const EntityContext<Statement> followsContext;
    const EntityContext<Statement> modifiesContext;
    const EntityContext<Statement> usesContext;

    ExtractionContext() = default;

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();

    EntityContext<Procedure> getProcedureContext();
    EntityContext<Statement> getFollowsContext();
    EntityContext<Statement> getModifiesContext();
    EntityContext<Statement> getParentContext();
    EntityContext<Statement> getUsesContext();
};
