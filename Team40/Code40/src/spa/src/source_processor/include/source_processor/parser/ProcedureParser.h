#pragma once

#include "common/model/Procedure.h"

#include "source_processor/parser/EntityParser.h"

using namespace std;

class ProcedureParser : public EntityParser<Procedure> {
public:
    ProcedureParser(vector<string> content);
    Procedure *parseEntity();
};
