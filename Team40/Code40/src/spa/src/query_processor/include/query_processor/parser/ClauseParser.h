#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/ParserUtil.h"

using namespace std;

class ClauseParser {
public:
    ClauseParser() = default;

    void initReferences(vector<Reference *> &declList);

    Clause *parse(ClsTuple cls) { return parseSt(cls); }
    Clause *parse(PatTuple cls) { return parsePt(cls); }
    Clause *parse(WithPair cls) { return parseWt(cls); }

    virtual void clear() = 0;

protected:
    virtual Clause *parseSt(ClsTuple);
    virtual Clause *parsePt(PatTuple);
    virtual Clause *parseWt(WithPair);

    Reference *getReferenceIfDeclared(string syn);

    vector<Reference *> declList;

    static DesignEntityTypeHelper deHelper;
    static ClauseTypeHelper clsHelper;
};
