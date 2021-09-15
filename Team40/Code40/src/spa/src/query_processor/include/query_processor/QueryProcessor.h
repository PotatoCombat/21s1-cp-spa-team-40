#pragma once

#include <algorithm>
#include <iterator>
#include <list>
#include <string>
#include <iostream>

#include "QueryEvaluator.h"
#include "QueryPreprocessor.h"
#include "query_processor/preprocessor/PreprocessorException.h"


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
