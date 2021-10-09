#include "query_processor/QueryEvaluator.h"

void QueryEvaluator::clear() {
    resultTable.clear();
    returnRefs.clear();
    references.clear();
    clauses.clear();
    referenceAppearInClauses.clear();
}

QueryEvaluator::QueryEvaluator(PKB *pkb)
    : pkb(pkb) {}

vector<string> QueryEvaluator::evaluateQuery(Query query) {
    try {
        clear();
        returnRefs = query.getReturnReferences();
        references = query.getReferences();
        clauses = query.getClauses();
        resultTable.init(references.size());
        vector<bool> referenceAppearInClauses(references.size(), false);
        this->referenceAppearInClauses = referenceAppearInClauses;

        bool exitEarly = false;

        evalClauses(exitEarly);

        return finaliseResult(exitEarly);

    } catch (ClauseHandlerError &e) {
        // to be implemented later
        return vector<string>{};
    }
}

void QueryEvaluator::evalPattern(bool &exitEarly) {
    for (PatternClause *pattern : patterns) {
        Result tempResult;
        AssignPatternHandler *patternHandler;

        if (pattern->getStmt()->getDeType() == DesignEntityType::ASSIGN) {
            AssignPatternHandler handler(pattern, pkb);
            patternHandler = &handler;
        }

        // eval and combine result
        int ref1Index = -1, ref2Index = -1;
        if (pattern->getStmt()->getRefType() == ReferenceType::SYNONYM) {
            ref1Index = getRefIndex(pattern->getStmt());
        }
        if (pattern->getVar()->getRefType() == ReferenceType::SYNONYM) {
            ref2Index = getRefIndex(pattern->getVar());
        }
        tempResult = patternHandler->eval();
        combineResult(tempResult, ref1Index, ref2Index, exitEarly);
        if (exitEarly) {
            return;
        }
    }
}

void QueryEvaluator::evalSuchThat(bool &exitEarly) {
    // handle clauses
    for (Clause *clause : clauses) {
        Result tempResult;

        ClauseHandler *clauseHandler;

        if (clause->getType() == ClauseType::FOLLOWS) {
            FollowsHandler followsHandler(clause, pkb);
            clauseHandler = &followsHandler;
        }

        if (clause->getType() == ClauseType::FOLLOWS_T) {
            FollowsStarHandler followsStarHandler(clause, pkb);
            clauseHandler = &followsStarHandler;
        }

        if (clause->getType() == ClauseType::PARENT) {
            ParentHandler parentHandler(clause, pkb);
            clauseHandler = &parentHandler;
        }

        if (clause->getType() == ClauseType::PARENT_T) {
            ParentStarHandler parentStarHandler(clause, pkb);
            clauseHandler = &parentStarHandler;
        }

        if (clause->getType() == ClauseType::MODIFIES_P) {
            ModifiesProcHandler modifiesProcHandler(clause, pkb);
            clauseHandler = &modifiesProcHandler;
        }

        if (clause->getType() == ClauseType::MODIFIES_S) {
            ModifiesStmtHandler modifiesStmtHandler(clause, pkb);
            clauseHandler = &modifiesStmtHandler;
        }

        if (clause->getType() == ClauseType::USES_P) {
            UsesProcHandler usesProcHandler(clause, pkb);
            clauseHandler = &usesProcHandler;
        }

        if (clause->getType() == ClauseType::USES_S) {
            UsesStmtHandler usesStmtHandler(clause, pkb);
            clauseHandler = &usesStmtHandler;
        }

        int ref1Index = -1, ref2Index = -1;
        if (clause->getFirstReference()->getRefType() == ReferenceType::SYNONYM) {
            ref1Index = getRefIndex(clause->getFirstReference());
        }
        if (clause->getSecondReference()->getRefType() == ReferenceType::SYNONYM) {
            ref2Index = getRefIndex(clause->getSecondReference());
        }

        // eval and combine result
        tempResult = clauseHandler->eval();
        combineResult(tempResult, ref1Index, ref2Index, exitEarly);

        if (exitEarly) {
            return;
        }
    }
}

vector<string> QueryEvaluator::finaliseResult(bool exitEarly) {
    vector<string> trueRes{"TRUE"};
    vector<string> falseRes{"FALSE"};
    vector<string> emptyRes = vector<string>{};

    // handle exit early cases
    if (exitEarly) {
        if (returnRefs.empty()) {
            return falseRes;
        } else {
            return emptyRes;
        }
    }

    // if no return ref but not exit early then return TRUE
    if (returnRefs.empty()) {
        return trueRes;
    }

    // for values that is returned but not appear in any clauses, 
    // fill in the table with all possible values
    vector<int> returnIndexes;
    for (auto ref : returnRefs) {
        int idx = getRefIndex(ref);
        returnIndexes.push_back(idx);
        if (!referenceAppearInClauses[idx]) {
            for (string val : ClauseHandler::getAll(pkb, *ref)) {
                resultTable.addValue(idx, val);
            }
        }
    }

    // generate and format result
    vector<vector<string>> unformattedRes =
        resultTable.generateResult(returnIndexes);
    return ResultFormatter::formatResult(unformattedRes);
}

void QueryEvaluator::combineResult(Result result, int ref1Idx, int ref2Idx, bool &exitEarly) {
    if (!result.isResultValid()) {
        exitEarly = true;
        return;
    }

    map<VALUE, VALUE_SET> res1;
    map<VALUE, VALUE_SET> res2;
    vector<pair<int, string>> toRemove;

    if (result.hasResultList1() && result.hasResultList2()) {
        if (referenceAppearInClauses[ref1Idx] && referenceAppearInClauses[ref2Idx]) {
            // ref1Idx, ref2Idx should be >= 0
            res1 = result.getResultList1();
            res2 = result.getResultList2();
            // remove links
            for (auto it = res1.begin(); it != res1.end();) {
                string sourceVal = it->first;
                VALUE_SET vals = it->second;
                bool erased = false;
                for (string targetVal : vals) {
                    if (!resultTable.hasLink(ref1Idx, sourceVal, 
                                             ref2Idx, targetVal)) {
                        res1[sourceVal].erase(targetVal);
                        if (res1[sourceVal].size() == 0) {
                            it = res1.erase(it);
                            erased = true;
                        } 
                    }
                }
                if (!erased) {
                    ++it;
                }
            }
            for (auto it = res2.begin(); it != res2.end();) {
                string sourceVal = it->first;
                VALUE_SET vals = it->second;
                bool erased = false;
                for (string targetVal : vals) {
                    if (!resultTable.hasLink(ref2Idx, sourceVal,
                                             ref1Idx, targetVal)) {
                        res2[sourceVal].erase(targetVal);
                        if (res2[sourceVal].size() == 0) {
                            it = res2.erase(it);
                            erased = true;
                        } 
                    }
                }
                if (!erased) {
                    ++it;
                }
            }

            vector<string> existingValues = resultTable.getValues(ref1Idx);
            for (string sourceVal : resultTable.getValues(ref1Idx)) {
                VALUE_SET vals =
                    resultTable.getPointersToIdx(ref1Idx, sourceVal, ref2Idx);
                for (string targetVal : vals) {
                    bool hasLink = true;
                    if (res1.find(sourceVal) == res1.end()) {
                        hasLink = false;   
                    } else if (res1[sourceVal].find(targetVal) ==
                               res1[sourceVal].end()) {
                        hasLink = false;
                    }
                    if (!hasLink) {
                        resultTable.removeLink(ref1Idx, sourceVal, ref2Idx,
                                               targetVal);
                    }
                }
            }

            // add all
            for (auto &valueToVals : res1) {
                resultTable.addValueWithLink(ref1Idx, valueToVals.first,
                                             ref2Idx, valueToVals.second);
            }
            for (auto &valueToVals : res2) {
                resultTable.addValueWithLink(ref2Idx, valueToVals.first,
                                             ref1Idx, valueToVals.second);
            }

            // remove those with no link
            for (string refValue : resultTable.getValues(ref1Idx)) {
                if (!resultTable.hasPointerToIdx(ref1Idx, refValue, ref2Idx)) {
                    resultTable.removeValue(ref1Idx, refValue);
                }
            }
            for (string refValue : resultTable.getValues(ref2Idx)) {
                if (!resultTable.hasPointerToIdx(ref2Idx, refValue, ref1Idx)) {
                    resultTable.removeValue(ref2Idx, refValue);
                }
            }
            return;
        }
    }

    // second ref is wildcard
    if (result.hasResultList1()) {
        // ref1Idx should be >= 0
        res1 = result.getResultList1();
        if (referenceAppearInClauses[ref1Idx]) {
            for (auto &valueToValuesPair : res1) {
                string val = valueToValuesPair.first;
                if (!resultTable.hasVal(ref1Idx, val)) {
                    toRemove.push_back({ref1Idx, val});
                }
            }
            for (string val : resultTable.getValues(ref1Idx)) {
                if (res1.find(val) == res1.end()) {
                    toRemove.push_back({ref1Idx, val});
                }
            }
        }
        referenceAppearInClauses[ref1Idx] = true;
        for (auto &valueToValuesPair : res1) {
            if (ref2Idx < 0) {
                resultTable.addValue(ref1Idx, valueToValuesPair.first);
            } else {
                resultTable.addValueWithLink(ref1Idx, valueToValuesPair.first,
                                             ref2Idx, valueToValuesPair.second);
            }
            
        }
    }

    // first ref is wildcard
    if (result.hasResultList2()) {
        // ref2Idx should be >= 0
        res2 = result.getResultList2();
        if (referenceAppearInClauses[ref2Idx]) {
            for (auto &valueToValuesPair : res2) {
                string val = valueToValuesPair.first;
                if (!resultTable.hasVal(ref2Idx, val)) {
                    toRemove.push_back({ref2Idx, val});
                }
            }
            for (string val : resultTable.getValues(ref2Idx)) {
                if (res2.find(val) == res2.end()) {
                    toRemove.push_back({ref2Idx, val});
                }
            }
        }
        referenceAppearInClauses[ref2Idx] = true;
        for (auto &valueToValuesPair : res2) {
            if (ref1Idx < 0) {
                resultTable.addValue(ref2Idx, valueToValuesPair.first);
            } else {
                resultTable.addValueWithLink(ref2Idx, valueToValuesPair.first,
                                             ref1Idx, valueToValuesPair.second);
            }
        }
    }

    for (auto& mapCoord : toRemove) {
        resultTable.removeValue(mapCoord.first, mapCoord.second);
    }
    
    if (ref1Idx >= 0 && resultTable.empty(ref1Idx)) {
        exitEarly = true;
        return;
    }

    if (ref2Idx >= 0 && resultTable.empty(ref2Idx)) {
        exitEarly = true;
        return;
    }
}

int QueryEvaluator::getRefIndex(Reference *ref) {
    int index = -1;
    for (int i = 0; i < references.size(); i++) {
        if (references[i]->equals(*ref)) {
            index = i;
        }
    }
    return index;
}
