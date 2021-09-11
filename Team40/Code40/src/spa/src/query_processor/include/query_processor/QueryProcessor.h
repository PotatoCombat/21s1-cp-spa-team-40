#pragma once

#include <algorithm>
#include <iterator>
#include <list>
#include <string>

#include "QueryEvaluator.h"
#include "QueryPreprocessor.h"

using namespace std;

class QueryProcessor {
private:
    QueryPreprocessor preprocessor;
    QueryEvaluator evaluator;
    PKB *pkb;

public:
    QueryProcessor();

    void processQuery(string query, list<string> &results);
};
