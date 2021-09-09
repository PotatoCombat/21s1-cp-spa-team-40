#include "source_processor/ProcedureParser.h"
#include "common/model/Procedure.h"
#include <algorithm>

Procedure ProcedureParser::parseProcedure() {
    vector<string>::iterator procItr =
        find(content.begin(), content.end(), "procedure");
    string proc_name = *next(procItr);
    Procedure proc = Procedure(proc_name);
    return proc;
}
