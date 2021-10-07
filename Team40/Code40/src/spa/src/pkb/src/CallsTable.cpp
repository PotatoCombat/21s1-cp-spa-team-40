#include "pkb/CallsTable.h"

CallsTable::CallsTable() = default;

void CallsTable::insertCalls(Procedure *proc, ProcName called) {
    ProcName caller = proc->getName();

    auto searchForCalledProcs = calledMap.find(caller);
    if (searchForCalledProcs == calledMap.end()) {
        calledMap[caller] = { called };
    } else {
        searchForCalledProcs->second.insert(called);
    }

    auto searchForCallerProcs = callerMap.find(called);
    if (searchForCallerProcs == callerMap.end()) {
        callerMap[called] = { caller };
    } else {
        searchForCallerProcs->second.insert(caller);
    }
}

set<ProcName> CallsTable::getCalledProcs(ProcName caller) {
    auto searchForCalledProcs = calledMap.find(caller);
    if (searchForCalledProcs == calledMap.end()) {
        return { };
    }
    return searchForCalledProcs->second;
}

set<ProcName> CallsTable::getCallerProcs(ProcName called) {
    auto searchForCallerProcs = callerMap.find(called);
    if (searchForCallerProcs == callerMap.end()) {
        return { };
    }
    return searchForCallerProcs->second;
}

bool CallsTable::calls(ProcName caller, ProcName called) {
    auto searchForCalledProcs = calledMap.find(caller);
    if (searchForCalledProcs == calledMap.end()) {
        return false;
    }
    set<ProcName> calledProcs = searchForCalledProcs->second;
    return calledProcs.find(called) != calledProcs.end();
}
