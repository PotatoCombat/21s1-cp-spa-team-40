#pragma once
#include "common/model/Procedure.h"
#include "source_processor/Line.h"
#include <string>
#include <vector>

using namespace std;

class ProcedureParser {
protected:
    vector<string> content;

public:
    ProcedureParser(vector<string> content);
    Procedure *parseProcedure();
    bool isValidName(string input);
};
