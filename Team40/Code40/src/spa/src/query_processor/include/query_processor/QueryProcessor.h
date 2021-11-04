#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <string>

#include "query_processor/QueryEvaluator.h"
#include "query_processor/QueryOptimizer.h"
#include "query_processor/QueryPreprocessor.h"

using namespace std;

class QueryProcessor {
public:
    explicit QueryProcessor(PKB *pkb);

    void processQuery(string query, list<string> &results);

private:
    PKB *pkb;
    QueryPreprocessor preprocessor;
    QueryEvaluator evaluator = QueryEvaluator(nullptr);
};
