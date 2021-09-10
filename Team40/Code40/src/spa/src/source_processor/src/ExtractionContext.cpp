#include "source_processor/ExtractionContext.h"

ExtractionContext &ExtractionContext::getInstance() {
    static ExtractionContext instance;
    return instance;
}

EntityContext<Procedure> ExtractionContext::getProcedureContext() {
    return procedureContext;
}

EntityContext<Statement> ExtractionContext::getFollowsContext() {
    return followsContext;
}

EntityContext<Statement> ExtractionContext::getModifiesContext() {
    return modifiesContext;
}

EntityContext<Statement> ExtractionContext::getParentContext() {
    return parentContext;
}

EntityContext<Statement> ExtractionContext::getUsesContext() {
    return usesContext;
}