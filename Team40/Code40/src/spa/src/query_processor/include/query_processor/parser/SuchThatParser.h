#pragma once

#include <string>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityType.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/ParserUtil.h"

class SuchThatParser {
public:
    SuchThatParser();

    Clause *parse(ClsTuple clsTuple);

    void initReferences(vector<Reference *> &declList);
    void clear();

private:
    Clause *parseStmtStmt();
    Clause *parseXEnt();

    Reference *getReferenceIfDeclared(string syn);

    ClauseTypeHelper clsHelper;
    DesignEntityTypeHelper deHelper;

    vector<Reference *> declList;

    string type;
    string ref1;
    string ref2;
};
