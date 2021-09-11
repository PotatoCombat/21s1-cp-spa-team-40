#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "QueryParser.h"
#include "QueryTokenizer.h"

#include "query_processor/model/Query.h"

using namespace std;

class QueryPreprocessor {
private:
    QueryTokenizer tokenizer;
    QueryParser parser;

public:
    QueryPreprocessor() = default;
    void preprocessQuery(const string input, Query &q);
};
