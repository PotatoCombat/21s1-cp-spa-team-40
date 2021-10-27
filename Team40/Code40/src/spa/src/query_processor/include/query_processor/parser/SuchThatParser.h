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
    Clause *parseAffects();
    Clause *parseXEnt();
    Clause *parseModifies();
    Clause *parseUses();

    Reference *parseAssignRef(string syn);

    Reference *r1 = nullptr;
    Reference *r2 = nullptr;
    string type;
    string ref1;
    string ref2;
};
