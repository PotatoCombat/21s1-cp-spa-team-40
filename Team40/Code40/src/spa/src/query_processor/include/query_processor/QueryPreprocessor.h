#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "QueryParser.h"
#include "QueryTokenizer.h"

#include "query_processor/model/Query.h"
#include "query_processor/exception/ValidityError.h"

using namespace std;

class QueryPreprocessor {
private:
    QueryTokenizer tokenizer;
    QueryParser parser;

public:
    QueryPreprocessor() = default;
    bool preprocessQuery(string input, Query &q);
};
