#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/model/Query.h"

using namespace std;

class QueryOptimizer {
private:
    static const int INVALID_IDX = -1;

    // Returns a list of <group idx, group size>
    static vector<pair<int, int>> formGroups(vector<Clause *> &unsortedClauses,
                                             map<string, int> &refToGroup);

    /* Updates group size of the 2 groups connected by cls
    * Returns a vector containing idx1, idx2 and newGroupIdx
    */ 
    static vector<int> updateGroupSize(Clause *cls,
                                       vector<pair<int, int>> &groupSizes,
                                       map<string, int> &refToGroup);

    // merges 2 groups with idx1 and idx2 that are connected by cls into a new group with newGroupIdx
    static void mergeTwoGroups(int newGroupIdx, int idx1, int idx2, Clause *cls,
                               map<string, int> &refToGroup);

    static vector<Clause *> reorderClause(vector<Clause *> &unsortedClauses,
                                          vector<pair<int, int>> &groupSizes,
                                          map<string, int> &refToGroup);
    
    static void populateNewQueryObject(vector<Clause *> sortedClauses, Query &before, Query &after);

public:
    static void optimize(Query &before, Query &after);
};
