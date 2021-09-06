#pragma once

#include "model/Query.h"
#include <string>
#include <vector>

using namespace std;

class QueryEvaluator {
public:
    vector<string> evaluateQuery(Query query);
};
