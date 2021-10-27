#pragma once

#include "query_processor/model/ClauseType.h"
#include "query_processor/Result.h"
#include "query_processor/ResultTable.h"

#include <map>
#include <stdexcept>
#include <vector>

typedef map<VALUE, VALUE_SET> VALUES_MAP;

using namespace std;

class ResultCache {
private:
    static const int nClauseTypes = 16;

    vector<bool> hasCacheList;

    vector<VALUES_MAP> mapR1ToR2s;
    vector<VALUES_MAP> mapR2ToR1s;

    void cache(Result res, int idx);
    bool hasCache(int idx);
    void setHasCache(int idx);
    VALUE_SET getR1Values(VALUE r2, int idx);
    VALUE_SET getR2Values(VALUE r1, int idx);
    bool isR1ClauseR2(VALUE r1, VALUE r2, int idx);

    void assertNonEmptyCache(int idx);

public:
    explicit ResultCache();

    void cache(Result res, ClauseType clsType);
    bool hasCache(ClauseType clsType);
    VALUE_SET getR1Values(VALUE r2, ClauseType clsType);
    VALUE_SET getR2Values(VALUE r1, ClauseType clsType);
    bool isR1ClauseR2(VALUE r1, VALUE r2, ClauseType clsType);
};
