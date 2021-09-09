#include "source_processor/ProcedureParser.h"
#include <algorithm>

ProcedureParser::ProcedureParser(vector<string> content) : content(content) {}

Procedure ProcedureParser::parseProcedure() {
    vector<string>::iterator procItr =
        find(content.begin(), content.end(), "procedure");
    string proc_name = *next(procItr);
    Procedure proc = Procedure(proc_name);
    return proc;
}
