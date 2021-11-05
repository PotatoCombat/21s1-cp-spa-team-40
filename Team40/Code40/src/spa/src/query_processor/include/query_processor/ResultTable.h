#pragma once

#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

typedef string VALUE;
typedef int INDEX;
typedef set<VALUE> VALUE_SET;
typedef map<INDEX, VALUE_SET> IDX_TO_VALUES_MAP;
typedef map<string, IDX_TO_VALUES_MAP> VALUE_TO_POINTERS_MAP;

class ResultTable {
private:
    static const string EMPTY;

    vector<VALUE_TO_POINTERS_MAP> table;

    void assertIndex(INDEX idx);

    /*
     * Calculates all the combination result of the group that contains the
     * input index if the group hasn't been evaluated yet
     */
    void appendGroupResult(set<INDEX> &visited,
                           vector<vector<string>> &existingCombinations,
                           INDEX idx, vector<INDEX> &returnIdexes);

    void constructNewCombinations(vector<vector<string>> &newCombinations,
                                  vector<vector<string>> &existingCombinations,
                                  set<INDEX> &visitedInGroup, INDEX idx);

    bool canAppendValue(VALUE value, INDEX idx,
                        vector<string> &combination,
                        set<INDEX> &visistedInGroup);

public:
    void clear();

    void init(int size);

    void addValue(INDEX idx, VALUE val);

    void addValueWithLink(INDEX sourceIdx, VALUE sourceVal, INDEX targetIdx, VALUE_SET targetVals);

    VALUE_SET getLinkedValues(INDEX sourceIdx, VALUE value, INDEX targetIdx);

    vector<INDEX> getLinkedIndexes(INDEX idx);

    bool hasPointerToIdx(INDEX sourceIdx, VALUE sourceValue, INDEX targetIdx);

    VALUE_SET getPointersToIdx(INDEX sourceIdx, VALUE sourceValue,
                               INDEX targetIdx);

    void removeValue(INDEX refIndex, VALUE value);

    vector<VALUE> getValues(INDEX refIndex);

    bool isColumnEmpty(INDEX refIndex);

    bool hasLinkBetweenValues(INDEX refIndex1, VALUE value1, INDEX refIndex2, VALUE value2);

    void removeLink(INDEX refIndex1, VALUE value1, INDEX refIndex2, VALUE value2);

    bool hasVal(INDEX idx, VALUE val);

    vector<vector<string>> generateResult(vector<INDEX> indexes);
};
