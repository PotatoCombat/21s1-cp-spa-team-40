#include "source_processor/design_extractor/ExtractionContext.h"

ExtractionContext &ExtractionContext::getInstance() {
    static ExtractionContext instance;
    return instance;
}

EntityContext<Statement> ExtractionContext::getFollowsContext() {
    return followsContext;
}

EntityContext<Statement> ExtractionContext::getParentContext() {
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
    procDependencyMap[caller].insert(callee);
    procIndegreesCounter[callee]++;
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
        copy(procDependencyMap[c].begin(), procDependencyMap[c].end(),
             back_inserter(callers));
    }
    return false;
}

unordered_set<ProcName>
ExtractionContext::getProcDependencies(ProcName caller) {
    return procDependencyMap[caller];
}

// Uses Kahn's algorithm to sort procedures by dependencies
vector<ProcName> ExtractionContext::getTopologicallySortedProcNames() {
    vector<ProcName> sortedProcNames;
    vector<ProcName> callers;
    // Add all procedures with 0 in-degrees into callers
    for (auto &it : procDependencyMap) {
        ProcName caller = it.first;
        if (procIndegreesCounter[caller] == 0) {
            callers.push_back(caller);
        }
    }
    while (!callers.empty()) {
        ProcName c = callers.back();
        callers.pop_back();
        sortedProcNames.push_back(c);
        for (ProcName procName : procDependencyMap[c]) {
            procIndegreesCounter[procName]--;
            if (procIndegreesCounter[procName] == 0) {
                callers.push_back(procName);
            }
        }
    }
    return sortedProcNames;
}

void ExtractionContext::resetTransientContexts() {
    followsContext.reset();
    parentContext.reset();
    currentProcedure.reset();
    usingStatement.reset();
    modifyingStatement.reset();
}

void ExtractionContext::reset() {
    resetTransientContexts();
    procDependencyMap.clear();
    procIndegreesCounter.clear();
}