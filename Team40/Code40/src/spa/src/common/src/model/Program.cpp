#include "common/model/Program.h"

Program::Program() : procLst({}) {}

void Program::addToProcLst(Procedure *proc) { this->procLst.push_back(proc); }

vector<Procedure *> Program::getProcLst() { return this->procLst; };
