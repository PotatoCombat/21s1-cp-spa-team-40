#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/model/Query.h"

using namespace std;

class QueryOptimizer {
public:
    static void optimize(Query &before, Query &after);
};
