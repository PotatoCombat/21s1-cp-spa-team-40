#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "Query.h"
#include "QueryParser.h"
#include "QueryTokenizer.h"

using namespace std;

class QueryPreprocessor {
private:
    QueryTokenizer tokenizer;
    QueryParser parser;

public:
    QueryPreprocessor() = default;
    void preprocessQuery(const string input, Query &q);
};
