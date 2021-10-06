#include "source_processor/parser/ProcedureParser.h"
#include <algorithm>

ProcedureParser::ProcedureParser(vector<string> content) : content(content) {}

Procedure *ProcedureParser::parseProcedure() {
    vector<string>::iterator procItr = find(content.begin(), content.end(), "procedure");
    string proc_name = *next(procItr);
    if (!isValidName(proc_name)) {
        throw invalid_argument("invalid procedure name");
    }
    // procedure: 'procedure' proc_name '{' stmtLst '}'
    if (*next(next(procItr)) != "{") {
        throw invalid_argument("invalid procedure");
    }
    auto procedure = new Procedure(proc_name);
    return procedure;
}

bool ProcedureParser::isValidName(string input) {
    // NAME: LETTER (LETTER | DIGIT)*
    // procedure names and variables are strings of letters, and digits,
    // starting with a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(), [](char c) { return !(isalnum(c)); }) == input.end();
}
