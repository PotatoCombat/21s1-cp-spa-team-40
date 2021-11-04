#pragma once

#include <vector>

#include "common/model/Procedure.h"

using namespace std;

class Program {
private:
    vector<Procedure *> procLst;

public:
    Program();

    void addToProcLst(Procedure *proc);

    vector<Procedure *> getProcLst();
};
