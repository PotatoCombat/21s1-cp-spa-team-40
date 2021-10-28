#include "source_processor/parser/ProcedureParser.h"
#include <algorithm>

ProcedureParser::ProcedureParser(vector<string> content) : EntityParser(content, 0) {}

Procedure *ProcedureParser::parseEntity() {
    vector<string>::iterator procItr = find(content.begin(), content.end(), "procedure");
    if (next(procItr) == content.end()) {
        throw invalid_argument("invalid procedure");
    }
    string proc_name = *next(procItr);
    if (!isValidName(proc_name)) {
        throw invalid_argument("invalid procedure name");
    }
    // procedure: 'procedure' proc_name '{' stmtLst '}'
    if (next(next(procItr)) == content.end()) {
        throw invalid_argument("invalid procedure");
    }
    if (*next(next(procItr)) != "{") {
        throw invalid_argument("invalid procedure");
    }
    auto procedure = new Procedure(proc_name);
    return procedure;
}
