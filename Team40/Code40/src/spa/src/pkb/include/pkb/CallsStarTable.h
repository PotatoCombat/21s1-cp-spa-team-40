#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Procedure.h"
#include <map>

class CallsStarTable {

public:
    CallsStarTable();

    /// Stores Calls*(proc1, proc2).
    void insertCallsStar(Procedure *proc, ProcName called);

    /// Selects p such that Calls*(caller, p).
    set<ProcName> getCalledStarProcs(ProcName caller);

    /// Selects p such that Calls(p, called).
    set<ProcName> getCallerStarProcs(ProcName called);

    /// Selects BOOLEAN such that Calls(procName1, procName2).
    bool callsStar(ProcName caller, ProcName called);

private:
    /// Stores p such that Calls(caller, s).
    map<ProcName, set<ProcName>> calledStarMap;

    /// Stores p such that Calls(p, called).
    map<ProcName, set<ProcName>> callerStarMap;

    void insertIntoMaps(ProcName caller, ProcName called);
};
