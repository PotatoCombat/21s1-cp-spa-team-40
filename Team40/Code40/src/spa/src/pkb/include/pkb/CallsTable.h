#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Procedure.h"
#include <map>

class CallsTable {

public:
    CallsTable();

    /// Stores Calls(proc1, proc2).
    void insertCalls(Procedure *proc, ProcName called);

    /// Selects p such that Calls(caller, p).
    set<ProcName> getCalledProcs(ProcName caller);

    /// Selects p such that Calls(p, called).
    set<ProcName> getCallerProcs(ProcName called);

    /// Selects BOOLEAN such that Calls(procName1, procName2).
    bool calls(ProcName caller, ProcName called);

private:
    /// Stores p such that Calls(caller, s).
    map<ProcName, set<ProcName>> calledMap;

    /// Stores p such that Calls(p, called).
    map<ProcName, set<ProcName>> callerMap;
};
