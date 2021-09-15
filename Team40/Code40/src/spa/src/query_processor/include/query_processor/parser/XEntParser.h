#pragma once

#include <string>

#include "query_processor/Abstractions.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/ParserUtil.h"

using namespace std;

// the X in XEntParser is used to represent either a Stmt or Ent
// since it is not clear which it is at first glance, whether to
// process the first argument of a Modifies/Uses clause as
// a statement or a procedure
class XEntParser {
private:
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;
    vector<Reference *> declList;
    string type;
    string ref1;
    string ref2;

public:
    XEntParser(ClsTuple cls, vector<Reference *> &declList,
               DesignEntityTypeHelper &deH, ClauseTypeHelper &clsH);
    Clause *parse();
};
