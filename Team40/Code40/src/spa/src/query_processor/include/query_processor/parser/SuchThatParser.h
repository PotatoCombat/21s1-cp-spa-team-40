#pragma once

#include <string>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"

class SuchThatParser {
public:
    SuchThatParser();

    void initReferences(vector<Reference *> &declList);
    void clear();
    Clause *parse(ClsTuple clsTuple);

private:
    Clause *parseStmtStmt();
    Clause *parseXEnt();

    vector<Reference *> declList;

    string type;
    string ref1;
    string ref2;
};
