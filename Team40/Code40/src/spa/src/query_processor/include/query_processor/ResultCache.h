#pragma once

#include "query_processor/ResultTable.h"

#include "query_processor/model/ClauseType.h"
#include "query_processor/model/Result.h"

#include <map>
#include <stdexcept>
#include <vector>

typedef map<VALUE, VALUE_SET> VALUES_MAP;

using namespace std;

class ResultCache {
private:
    static const int nClauseTypes = static_cast<int>(ClauseType::Count);

    vector<VALUE_SET> fullyCachedR1s;
    vector<VALUE_SET> fullyCachedR2s;

    vector<VALUES_MAP> mapR1ToR2s;
    vector<VALUES_MAP> mapR2ToR1s;

    bool isFullyCached(string r, int idx, bool isR1);
    void setFullyCached(string r, int idx, bool isR1);

    void setFullyCachedAllValues(int idx);

    void cache(string r1, string r2, int idx);
    VALUE_SET getR1Values(VALUE r2, int idx);
    VALUE_SET getR2Values(VALUE r1, int idx);
    bool isR1ClauseR2(VALUE r1, VALUE r2, int idx);

    void assertCache(int idx);

public:
    explicit ResultCache();

    bool isR1FullyCached(string r1, ClauseType clsType);
    bool isR2FullyCached(string r2, ClauseType clsType);

    void setR1FullyCached(string r1, ClauseType clsType);
    void setR2FullyCached(string r1, ClauseType clsType);

    void setFullyCachedAllValues(ClauseType clsType);

    void cache(string r1, string r2, ClauseType clsType);
    VALUE_SET getR1Values(VALUE r2, ClauseType clsType);
    VALUE_SET getR2Values(VALUE r1, ClauseType clsType);
    bool isR1ClauseR2(VALUE r1, VALUE r2, ClauseType clsType);

    void clear();
};
