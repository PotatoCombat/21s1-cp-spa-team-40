#pragma once

#include <string>
#include <vector>

#include "query_processor/parser/ClauseParser.h"

class SuchThatParser : public ClauseParser {
public:
    SuchThatParser() : ClauseParser() {}

    void clear();

private:
    Clause *parseSt(ClsTuple clsTuple) override;

    Clause *parseStmtStmt();
    Clause *parseProcProc();
    Clause *parseXEnt();
    Clause *parseModifies();
    Clause *parseUses();

    string type;
    string ref1;
    string ref2;
};
