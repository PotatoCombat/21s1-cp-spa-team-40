#include "source_processor/design_extractor/ExtractionContext.h"

ExtractionContext &ExtractionContext::getInstance() {
    static ExtractionContext instance;
    return instance;
}

EntityContext<struct Statement> &ExtractionContext::getFollowsContext() {
    static EntityContext<Statement> followsContext;
    return followsContext;
}

EntityContext<struct Statement> &ExtractionContext::getParentContext() {
    static EntityContext<Statement> parentContext;
    return parentContext;
}

optional<Statement *> ExtractionContext::getModifyingStatement() {
    return modifyingStatement;
}

void ExtractionContext::setModifyingStatement(Statement *statement) {
    if (modifyingStatement.has_value()) {
        throw runtime_error("Trying to overwrite another modifying statement.");
    }
    modifyingStatement = statement;
}

void ExtractionContext::unsetModifyingStatement(Statement *statement) {
    if (!modifyingStatement.has_value()) {
        throw runtime_error("Trying to unset a null value.");
    }
    if (modifyingStatement.value() != statement) {
        throw runtime_error("Trying to unset another modifying statement.");
    }
    modifyingStatement = nullopt;
}

optional<Procedure *> ExtractionContext::getCurrentProcedure() {
    return currentProcedure;
}

void ExtractionContext::setCurrentProcedure(Procedure *procedure) {
    if (currentProcedure.has_value()) {
        throw runtime_error("Trying to overwrite another procedure.");
    }
    currentProcedure = procedure;
}

void ExtractionContext::unsetCurrentProcedure(Procedure *procedure) {
    if (!currentProcedure.has_value()) {
        throw runtime_error("Trying to unset a null value.");
    }
    if (currentProcedure.value() != procedure) {
        throw runtime_error("Trying to unset another procedure.");
    }
    currentProcedure = nullopt;
}

optional<Statement *> ExtractionContext::getUsingStatement() {
    return usingStatement;
}

void ExtractionContext::setUsingStatement(Statement *statement) {
    if (usingStatement.has_value()) {
        throw runtime_error("Trying to overwrite another using statement.");
    }
    usingStatement = statement;
}

void ExtractionContext::unsetUsingStatement(Statement *statement) {
    if (!usingStatement.has_value()) {
        throw runtime_error("Trying to unset a null value.");
    }
    if (usingStatement.value() != statement) {
        throw runtime_error("Trying to unset another using statement.");
    }
    usingStatement = nullopt;
}

void ExtractionContext::addProcDependency(ProcName from, ProcName to) {
    // Note: We are guaranteed that there will be no circular dependencies in
    // SIMPLE (i.e. recursion)
    procDependencyTree[from].insert(to);
}

unordered_set<ProcName> ExtractionContext::getProcDependencies(ProcName from) {
    return procDependencyTree[from];
}