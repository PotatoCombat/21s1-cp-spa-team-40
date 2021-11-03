#include "source_processor/design_extractor/ExtractionContext.h"
#include <iterator>
#include <utility>

ExtractionContext &ExtractionContext::getInstance() {
    static ExtractionContext instance;
    return instance;
}

optional<Procedure *> ExtractionContext::getCurrentProcedure() {
    return currentProcedure;
}

void ExtractionContext::setCurrentProcedure(Procedure *procedure) {
    if (currentProcedure.has_value()) {
        throw runtime_error("Trying to overwrite another procedure.");
    }
    if (procDependencyMap.find(procedure->getId()) ==
        procDependencyMap.end()) {
        procDependencyMap[procedure->getId()] = unordered_set<ProcName>();
        procIndegreesCounter[procedure->getId()] = 0;
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

StmtIndex
ExtractionContext::getFirstExecutableStatement(const ProcName &procName) {
    if (procFirstExecutableStatements.find(procName) ==
        procFirstExecutableStatements.end()) {
        throw runtime_error("Trying to get a null value.");
    }
    return procFirstExecutableStatements[procName];
}

void ExtractionContext::setFirstExecutableStatement(const ProcName &procName,
                                                    StmtIndex stmtIndex) {
    if (procFirstExecutableStatements.count(procName)) {
        throw runtime_error("Trying to unset another StmtIndex.");
    }
    procFirstExecutableStatements[procName] = stmtIndex;
}

set<StmtIndex>
ExtractionContext::getLastExecutableStatements(const ProcName &procName) {
    if (procLastExecutableStatements.find(procName) ==
        procLastExecutableStatements.end()) {
        throw runtime_error("Trying to get a null value.");
    }
    return procLastExecutableStatements[procName];
}

void ExtractionContext::setLastExecutableStatements(
    const ProcName &procName, set<StmtIndex> stmtIndices) {
    procLastExecutableStatements[procName] = std::move(stmtIndices);
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

vector<Statement *> ExtractionContext::getParentStatements() {
    return parentStatements;
}

void ExtractionContext::setParentStatement(Statement *statement) {
    parentStatements.push_back(statement);
}

void ExtractionContext::unsetParentStatement(Statement *statement) {
    if (parentStatements.empty()) {
        throw runtime_error("Trying to unset a null value.");
    }
    if (parentStatements.back() != statement) {
        throw runtime_error("Trying to unset another parent statement.");
    }
    parentStatements.pop_back();
}

void ExtractionContext::clearParentStatements() { parentStatements.clear(); }

vector<Statement *> ExtractionContext::getPrecedingStatements() {
    return precedingStatements;
}

void ExtractionContext::setPrecedingStatement(Statement *statement) {
    precedingStatements.push_back(statement);
}

void ExtractionContext::unsetPrecedingStatement(Statement *statement) {
    if (precedingStatements.empty()) {
        throw runtime_error("Trying to unset a null value.");
    }
    if (precedingStatements.back() != statement) {
        throw runtime_error("Trying to unset another previous statement.");
    }
    precedingStatements.pop_back();
}

void ExtractionContext::clearPrecedingStatements() {
    precedingStatements.clear();
}

vector<Statement *> ExtractionContext::getPreviousStatements() {
    return previousStatements;
}

void ExtractionContext::setPreviousStatement(Statement *statement) {
    previousStatements.push_back(statement);
}

void ExtractionContext::unsetPreviousStatement(Statement *statement) {
    if (previousStatements.empty()) {
        throw runtime_error("Trying to unset a null value.");
    }
    if (previousStatements.back() != statement) {
        throw runtime_error("Trying to unset another previous statement.");
    }
    previousStatements.pop_back();
}

void ExtractionContext::clearPreviousStatements() {
    previousStatements.clear();
}

void ExtractionContext::registerProcDependency(const ProcName &caller,
                                               const ProcName &callee) {
    // Note: We are guaranteed that there will be no circular dependencies in
    // SIMPLE (i.e. recursion)
    if (hasCyclicalProcDependency(caller, callee)) {
        throw runtime_error("There exists a cyclical dependency between " +
                            caller + " and " + callee);
    }
    // If the dependency already exists, do nothing
    if (procDependencyMap[caller].count(callee)) {
        return;
    }
    procDependencyMap[caller].insert(callee);
    procIndegreesCounter[callee]++;
}

bool ExtractionContext::hasCyclicalProcDependency(const ProcName &caller,
                                                  const ProcName &callee) {
    if (caller == callee) {
        return true;
    }
    vector<ProcName> callers;
    callers.push_back(callee);
    while (!callers.empty()) {
        ProcName c = callers.back();
        if (c == caller) {
            return true;
        }
        callers.pop_back();
        copy(procDependencyMap[c].begin(), procDependencyMap[c].end(),
             back_inserter(callers));
    }
    return false;
}

unordered_set<ProcName>
ExtractionContext::getProcDependencies(const ProcName &caller) {
    return procDependencyMap[caller];
}

// Uses Kahn's algorithm to sort procedures by dependencies
vector<ProcName> ExtractionContext::getTopologicallySortedProcNames() {
    vector<ProcName> sortedProcNames;
    vector<ProcName> callers;
    unordered_map<ProcName, unordered_set<ProcName>> procDependencyMapCopy(
        procDependencyMap);
    unordered_map<ProcName, int> procIndegreesCounterCopy(procIndegreesCounter);
    // Add all procedures with 0 in-degrees into callers
    for (auto &it : procDependencyMapCopy) {
        ProcName caller = it.first;
        if (procIndegreesCounterCopy[caller] == 0) {
            callers.push_back(caller);
        }
    }
    while (!callers.empty()) {
        ProcName c = callers.back();
        callers.pop_back();
        sortedProcNames.push_back(c);
        for (ProcName procName : procDependencyMapCopy[c]) {
            procIndegreesCounterCopy[procName]--;
            if (procIndegreesCounterCopy[procName] == 0) {
                callers.push_back(procName);
            }
        }
    }
    return sortedProcNames;
}

void ExtractionContext::resetTransientContexts() {
    precedingStatements.clear();
    parentStatements.clear();
    currentProcedure.reset();
    usingStatement.reset();
    modifyingStatement.reset();
}

void ExtractionContext::reset() {
    resetTransientContexts();
    procDependencyMap.clear();
    procIndegreesCounter.clear();
    procFirstExecutableStatements.clear();
    procLastExecutableStatements.clear();
}