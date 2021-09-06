#pragma once

#include "QueryEvaluator.h"
#include "query_preprocessor/QueryPreprocessor.h"
#include <algorithm>
#include <iterator>
#include <list>
#include <string>

using namespace std;

class QueryProcessor {
private:
    QueryPreprocessor preprocessor;
    QueryEvaluator evaluator;

public:
    QueryProcessor();

    void processQuery(string query, list<string> &results);
};
