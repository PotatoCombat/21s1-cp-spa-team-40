#include "pkb/CallsStarTable.h"

CallsStarTable::CallsStarTable() = default;

void CallsStarTable::insertCallsStar(Procedure *proc, ProcName called) {
    ProcName caller = proc->getName();

    insertIntoMaps(caller, called);

    // Adding Calls*(callerProcs, proc2)
    auto searchForCallerProcs = callerStarMap.find(caller);
    if (searchForCallerProcs != callerStarMap.end()) {
        for (const ProcName &parentCaller : searchForCallerProcs->second) {
            insertIntoMaps(parentCaller, called);
        }
    }

    // Adding Calls*(proc1, calledProcs)
    auto searchForCalledProcs = calledStarMap.find(called);
    if (searchForCalledProcs != calledStarMap.end()) {
        for (const ProcName &childCalled : searchForCalledProcs->second) {
            insertIntoMaps(called, childCalled);
        }
    }
}

set<ProcName> CallsStarTable::getCalledStarProcs(ProcName caller) {
    auto searchForCalledProcs = calledStarMap.find(caller);
    if (searchForCalledProcs == calledStarMap.end()) {
        return { };
    }
    return searchForCalledProcs->second;
}

set<ProcName> CallsStarTable::getCallerStarProcs(ProcName called) {
    auto searchForCallerProcs = callerStarMap.find(called);
    if (searchForCallerProcs == callerStarMap.end()) {
        return { };
    }
    return searchForCallerProcs->second;
}

bool CallsStarTable::callsStar(ProcName caller, ProcName called) {
    auto searchForCalledProcs = calledStarMap.find(caller);
    if (searchForCalledProcs == calledStarMap.end()) {
        return false;
    }
    set<ProcName> calledProcs = searchForCalledProcs->second;
    return calledProcs.find(called) != calledProcs.end();
}

void CallsStarTable::insertIntoMaps(ProcName caller, ProcName called) {
    auto searchForCalledProcs = calledStarMap.find(caller);
    if (searchForCalledProcs == calledStarMap.end()) {
        calledStarMap[caller] = { called };
    }
    else {
        searchForCalledProcs->second.insert(called);
    }

    auto searchForCallerProcs = callerStarMap.find(called);
    if (searchForCallerProcs == callerStarMap.end()) {
        callerStarMap[called] = { caller };
    }
    else {
        searchForCallerProcs->second.insert(caller);
    }
}
