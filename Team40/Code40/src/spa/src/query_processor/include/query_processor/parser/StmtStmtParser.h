#pragma once

#include <string>

#include "query_processor/Abstractions.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"

using namespace std;

class StmtStmtParser {
private:
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;
    vector<Reference *> declList;
    string type;
    string ref1;
    string ref2;

public:
    StmtStmtParser(ClsTuple cls, vector<Reference *> &declList,
                  DesignEntityTypeHelper &deH, ClauseTypeHelper &clsH);
    Clause *parse();
};
