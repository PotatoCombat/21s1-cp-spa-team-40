#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "QueryParser.h"
#include "QueryTokenizer.h"

#include "query_processor/model/Query.h"
#include "query_processor/preprocessor/PreprocessorException.h"

using namespace std;

class QueryPreprocessor {
private:
    QueryTokenizer tokenizer;
    QueryParser parser;

public:
    QueryPreprocessor() = default;
    void preprocessQuery(string input, Query &q);
};
