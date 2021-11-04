#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "query_processor/QueryParser.h"
#include "query_processor/QueryTokenizer.h"

#include "query_processor/exception/ValidityError.h"

#include "query_processor/model/Query.h"

using namespace std;

class QueryPreprocessor {
public:
    QueryPreprocessor() = default;
    bool preprocessQuery(string input, Query &q);

private:
    void addReturnReferencesToQuery(vector<string> retStrings, Query &q);
    void clear();

    QueryTokenizer tokenizer;
    QueryParser parser;

    bool returnBoolean = false;

    inline static const string KEYWORD_BOOLEAN = "BOOLEAN";
};
