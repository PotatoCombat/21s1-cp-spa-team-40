#include "source_processor/parser/ProcedureParser.h"
#include <algorithm>

ProcedureParser::ProcedureParser(vector<string> content) : EntityParser(content, 0) {}

/**
 * Parses a tokenized string identified to be a procedure into a Procedure
 * object
 * @return Procedure object.
 */
Procedure *ProcedureParser::parseEntity() {
    vector<string>::iterator procItr =
        find(content.begin(), content.end(), Tokens::KEYWORD_PROCEDURE);
    if (next(procItr) == content.end()) {
        throw invalid_argument("invalid procedure");
    }
    string proc_name = *next(procItr);
    if (!isValidName(proc_name)) {
        throw invalid_argument("invalid procedure name");
    }
    // procedure: 'procedure' proc_name Tokens::CHAR_OPEN_BRACE stmtLst
    // Tokens::CHAR_CLOSE_BRACE
    if (next(next(procItr)) == content.end()) {
        throw invalid_argument("invalid procedure");
    }
    if (*next(next(procItr)) != Tokens::SYMBOL_OPEN_BRACE) {
        throw invalid_argument("invalid procedure");
    }
    entity = new Procedure(proc_name);
    return entity;
}
