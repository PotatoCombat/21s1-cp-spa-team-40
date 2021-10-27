#include "query_processor/ResultCache.h"

ResultCache::ResultCache() {
    hasCacheList = vector<bool>(nClauseTypes, false);
    mapR1ToR2s = vector<VALUES_MAP>(nClauseTypes, VALUES_MAP{});
    mapR2ToR1s = vector<VALUES_MAP>(nClauseTypes, VALUES_MAP{});
}

void ResultCache::cache(Result res, ClauseType clsType) {
    cache(res, static_cast<int>(clsType));
}

bool ResultCache::hasCache(ClauseType clsType) {
    return hasCache(static_cast<int>(clsType));
}

VALUE_SET ResultCache::getR1Values(VALUE r2, ClauseType clsType) {
    return getR1Values(r2, static_cast<int>(clsType));
}

VALUE_SET ResultCache::getR2Values(VALUE r1, ClauseType clsType) {
    return getR2Values(r1, static_cast<int>(clsType));
}

bool ResultCache::isR1ClauseR2(VALUE r1, VALUE r2, ClauseType clsType) {
    return isR1ClauseR2(r1, r2, static_cast<int>(clsType));
}

void ResultCache::cache(Result res, int idx) {
    if (hasCache(idx)) {
        return;
    }

    mapR1ToR2s[idx] = res.getResultList1();
    mapR2ToR1s[idx] = res.getResultList2();
    setHasCache(idx);
}

bool ResultCache::hasCache(int idx) { return hasCacheList[idx]; }

void ResultCache::setHasCache(int idx) { hasCacheList[idx] = true; }

VALUE_SET ResultCache::getR1Values(VALUE r2, int idx) {
    assertNonEmptyCache(idx);

    if (mapR2ToR1s[idx].find(r2) == mapR2ToR1s[idx].end()) {
        return VALUE_SET{};
    }

    return mapR2ToR1s[idx][r2];
}

VALUE_SET ResultCache::getR2Values(VALUE r1, int idx) {
    assertNonEmptyCache(idx);

    if (mapR1ToR2s[idx].find(r1) == mapR1ToR2s[idx].end()) {
        return VALUE_SET{};
    }

    return mapR2ToR1s[idx][r1];
}

bool ResultCache::isR1ClauseR2(VALUE r1, VALUE r2, int idx) {
    assertNonEmptyCache(idx);

    if (mapR1ToR2s[idx].find(r1) == mapR1ToR2s[idx].end()) {
        return false;
    }

    return mapR1ToR2s[idx][r1].count(r2) > 0;
}

void ResultCache::assertNonEmptyCache(int idx) {
    if (idx < 0 || idx >= nClauseTypes) {
        throw runtime_error("Cache index out of bound");
    }

    if (!hasCache(idx)) {
        throw runtime_error("Trying to access empty cache");
    }
}
