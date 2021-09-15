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

void ExtractionContext::addProcDependency(ProcName caller, ProcName callee) {
    // Note: We are guaranteed that there will be no circular dependencies in
    // SIMPLE (i.e. recursion)
    if (hasCyclicalProcDependency(caller, callee)) {
        throw runtime_error("There exists a cyclical dependency between " +
                            caller + " and " + callee);
    }
    procDependencyTree[caller].insert(callee);
}

bool ExtractionContext::hasCyclicalProcDependency(ProcName caller,
                                                  ProcName callee) {
    if (caller == callee) {
        return true;
    }
    unordered_set<ProcName> transitiveCallees;
    vector<ProcName> callers;
    callers.push_back(callee);
    while (!callers.empty()) {
        ProcName c = callers.back();
        if (c == caller) {
            return true;
        }
        callers.pop_back();
        transitiveCallees.insert(c);
        copy(procDependencyTree[c].begin(), procDependencyTree[c].end(),
             back_inserter(callers));
    }
    return false;
}

unordered_set<ProcName>
ExtractionContext::getProcDependencies(ProcName caller) {
    return procDependencyTree[caller];
}

vector<ProcName> ExtractionContext::getTopologicallySortedProcNames() {
    vector<ProcName> result;
    vector<ProcName> callers;
    // Add all top-level prcs into callers
    for (auto &it : procDependencyTree) {
        ProcName caller = it.first;
        callers.push_back(caller);
    }
    while (!callers.empty()) {
        ProcName c = callers.back();
        result.push_back(c);
        callers.pop_back();
        copy(procDependencyTree[c].begin(), procDependencyTree[c].end(),
             back_inserter(callers));
    }
    return result;
}

void ExtractionContext::resetTransientContexts() {
    followsContext.getAllEntities().clear();
    parentContext.getAllEntities().clear();
    currentProcedure.reset();
    usingStatement.reset();
    modifyingStatement.reset();
}

void ExtractionContext::reset() {
    resetTransientContexts();
    procDependencyTree.clear();
}