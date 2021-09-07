#pragma once
#include "Procedure.h"
#include <vector>
using namespace std;

class Program {
private:
    vector<Procedure> procLst;

public:
    Program();

    void addToProcLst(Procedure proc);

    vector<Procedure> getProcLst();
};
