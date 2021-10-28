#pragma once
#include "common/model/Procedure.h"
#include "source_processor/parser/EntityParser.h"
using namespace std;

class ProcedureParser : public EntityParser {
private:
    Procedure *proc;

public:
    ProcedureParser(vector<string> content);
    Procedure *parseEntity();
};
