#include "query_processor/ResultCache.h"

ResultCache::ResultCache() {
    fullyCachedR1s = vector<VALUE_SET>(nClauseTypes, VALUE_SET{});
    fullyCachedR2s = vector<VALUE_SET>(nClauseTypes, VALUE_SET{});
    mapR1ToR2s = vector<VALUES_MAP>(nClauseTypes, VALUES_MAP{});
    mapR2ToR1s = vector<VALUES_MAP>(nClauseTypes, VALUES_MAP{});
}

bool ResultCache::isR1FullyCached(string r1, ClauseType clsType) {
    return isFullyCached(r1, static_cast<int>(clsType), true);
}

bool ResultCache::isR2FullyCached(string r2, ClauseType clsType) {
    return isFullyCached(r2, static_cast<int>(clsType), false);
}

void ResultCache::setR1FullyCached(string r1, ClauseType clsType) {
    return setFullyCached(r1, static_cast<int>(clsType), true);
}

void ResultCache::setR2FullyCached(string r2, ClauseType clsType) {
    return setFullyCached(r2, static_cast<int>(clsType), false);
}

void ResultCache::setFullyCachedAllValues(ClauseType clsType) {
    return setFullyCachedAllValues(static_cast<int>(clsType));
}

void ResultCache::cache(string r1, string r2, ClauseType clsType) {
    cache(r1, r2, static_cast<int>(clsType));
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

void ResultCache::cache(string r1, string r2, int idx) {
    mapR1ToR2s[idx][r1].insert(r2);
    mapR2ToR1s[idx][r2].insert(r1);
}

VALUE_SET ResultCache::getR1Values(VALUE r2, int idx) {
    assertCache(idx);
    if (!isFullyCached(r2, idx, false)) {
        throw runtime_error("Not fully cached");
    }

    if (mapR2ToR1s[idx].find(r2) == mapR2ToR1s[idx].end()) {
        return VALUE_SET{};
    }
    return mapR2ToR1s[idx][r2];
}

VALUE_SET ResultCache::getR2Values(VALUE r1, int idx) {
    assertCache(idx);
    if (!isFullyCached(r1, idx, true)) {
        throw runtime_error("Not fully cached");
    }

    if (mapR1ToR2s[idx].find(r1) == mapR1ToR2s[idx].end()) {
        return VALUE_SET{};
    }

    return mapR1ToR2s[idx][r1];
}

bool ResultCache::isR1ClauseR2(VALUE r1, VALUE r2, int idx) {
    assertCache(idx);
    if (!isFullyCached(r1, idx, true)) {
        throw runtime_error("Not fully cached");
    }

    if (mapR1ToR2s[idx].find(r1) == mapR1ToR2s[idx].end()) {
        return false;
    }

    return mapR1ToR2s[idx][r1].count(r2) > 0;
}

bool ResultCache::isFullyCached(string r, int idx, bool isR1) {
    assertCache(idx);
    auto &fullyCachedRs = isR1 ? fullyCachedR1s : fullyCachedR2s;
    return fullyCachedRs[idx].count(r) > 0;
}

void ResultCache::setFullyCached(string r, int idx, bool isR1) {
    assertCache(idx);
    auto &fullyCachedRs = isR1 ? fullyCachedR1s : fullyCachedR2s;
    fullyCachedRs[idx].insert(r);
}

void ResultCache::setFullyCachedAllValues(int idx) { 
    assertCache(idx);
    for (auto &[key, value] : mapR1ToR2s[idx]) {
        setFullyCached(key, idx, true);
    }
    for (auto &[key, value] : mapR2ToR1s[idx]) {
        setFullyCached(key, idx, false);
    }
}

void ResultCache::assertCache(int idx) {
    if (idx < 0 || idx >= nClauseTypes) {
        throw runtime_error("Cache index out of bound");
    }
}

void ResultCache::clear() {
    fullyCachedR1s = vector<VALUE_SET>(nClauseTypes, VALUE_SET{});
    fullyCachedR2s = vector<VALUE_SET>(nClauseTypes, VALUE_SET{});
    mapR1ToR2s = vector<VALUES_MAP>(nClauseTypes, VALUES_MAP{});
    mapR2ToR1s = vector<VALUES_MAP>(nClauseTypes, VALUES_MAP{});
}
