#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <string>

#include "QueryEvaluator.h"
#include "QueryOptimizer.h"
#include "QueryPreprocessor.h"

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
