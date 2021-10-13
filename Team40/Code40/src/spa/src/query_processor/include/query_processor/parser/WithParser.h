#pragma once

#include <string>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/ParserUtil.h"

class WithParser {
public:
    WithParser();

    void initReferences(vector<Reference *> &declList);
    void clear();
    Clause *parse(WithPair withPair);

private:
    Reference *parseReference(string ref);

    Reference *getReferenceIfDeclared(string syn);

    DesignEntityTypeHelper deHelper;

    vector<Reference *> declList;

    string ref1;
    string ref2;
};
