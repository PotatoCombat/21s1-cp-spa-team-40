#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "QueryParser.h"
#include "QueryTokenizer.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Query.h"

using namespace std;

class QueryPreprocessor {
public:
    QueryPreprocessor() = default;
    bool preprocessQuery(string input, Query &q);

private:
    void addReturnReferenceToQuery(vector<string> retStrings, Query &q);

    QueryTokenizer tokenizer;
    QueryParser parser;
};
