#include "query_processor/QueryEvaluator.h"

void QueryEvaluator::clear() {
    resultTable.clear();
    returnRefs.clear();
    references.clear();
    clauses.clear();
    referenceAppearInClauses.clear();
    appearInSameClauseAlr.clear();
}

QueryEvaluator::QueryEvaluator(PKB *pkb)
    : pkb(pkb) {}

vector<string> QueryEvaluator::evaluateQuery(Query query) {
    try {
        clear();
        returnRefs = query.getReturnReferences();
        references = query.getReferences();
        clauses = query.getClauses();
        int refSize = references.size();
        resultTable.init(refSize);
        vector<bool> referenceAppearInClauses(refSize, false);
        this->referenceAppearInClauses = referenceAppearInClauses;
        vector<vector<bool>> areInSameClause(refSize,
                                             vector<bool>(refSize, false));
        this->appearInSameClauseAlr = areInSameClause;

        bool exitEarly = false;

        evalClauses(exitEarly);

        return finaliseResult(exitEarly);

    } catch (ClauseHandlerError &e) {
        // to be implemented later
        return vector<string>{};
    }
}

Result QueryEvaluator::getTempResult(Clause* clause) {
    ClauseHandler *clauseHandler;

    if (clause->getType() == ClauseType::PATTERN) {
        PatternHandler patternHandler(clause, pkb);
        clauseHandler = &patternHandler;
    }

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

    if (clause->getType() == ClauseType::CALLS) {
        CallsHandler callsHandler(clause, pkb);
        clauseHandler = &callsHandler;
    }

    if (clause->getType() == ClauseType::CALLS_T) {
        CallsStarHandler callsStarHandler(clause, pkb);
        clauseHandler = &callsStarHandler;
    }

    if (clause->getType() == ClauseType::NEXT) {
        NextHandler nextHandler(clause, pkb);
        clauseHandler = &nextHandler;
    }

    if (clause->getType() == ClauseType::NEXT_T) {
        NextStarHandler nextStarHandler(clause, pkb);
        clauseHandler = &nextStarHandler;
    }

    if (clause->getType() == ClauseType::WITH) {
        WithHandler withHandler(clause, pkb);
        clauseHandler = &withHandler;
    }

    if (clause->getType() == ClauseType::AFFECTS) {
        AffectsHandler affectsHandler(clause, pkb);
        clauseHandler = &affectsHandler;
    }

    if (clause->getType() == ClauseType::AFFECTS_T) {
        AffectsStarHandler affectsStarHandler(clause, pkb);
        clauseHandler = &affectsStarHandler;
    }

    return clauseHandler->eval();
}

void QueryEvaluator::evalClauses(bool &exitEarly) {
    // handle clauses
    for (Clause *clause : clauses) {
        Result tempResult = getTempResult(clause);

        int ref1Index = INVALID_INDEX, ref2Index = INVALID_INDEX;
        if (clause->getFirstReference()->getRefType() == ReferenceType::SYNONYM) {
            ref1Index = getRefIndex(clause->getFirstReference());
        }
        if (clause->getSecondReference()->getRefType() == ReferenceType::SYNONYM) {
            ref2Index = getRefIndex(clause->getSecondReference());
        }

        combineResult(tempResult, ref1Index, ref2Index, exitEarly);

        if (exitEarly) {
            break;
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

    vector<vector<string>> updatedAttrRes = handleAttr(unformattedRes);

    return ResultFormatter::formatResult(updatedAttrRes);
}

void QueryEvaluator::combineResult(Result result, int ref1Idx, int ref2Idx, bool &exitEarly) {
    if (!result.isResultValid()) {
        exitEarly = true;
        return;
    }

    if (result.hasResultList1() && result.hasResultList2() && appearInSameClauseAlr[ref1Idx][ref2Idx]) {
        combineTwoSyn(result, ref1Idx, ref2Idx);
    } else {
        vector<pair<int, string>> toRemove;

        // first ref is syn
        if (result.hasResultList1()) {
            // ref1Idx should not be INVALID_INDEX
            combineOneSyn(result, ref1Idx, ref2Idx, false, &toRemove);
        }

        // second ref is syn
        if (result.hasResultList2()) {
            // ref2Idx should not be INVALID_INDEX
            combineOneSyn(result, ref2Idx, ref1Idx, true, &toRemove);
        }

        for (auto &mapCoord : toRemove) {
            resultTable.removeValue(mapCoord.first, mapCoord.second);
        }
    }

    if (ref1Idx != INVALID_INDEX && ref2Idx != INVALID_INDEX) {
        appearInSameClauseAlr[ref1Idx][ref2Idx] = true;
        appearInSameClauseAlr[ref2Idx][ref1Idx] = true;
    }
    
    exitEarly = canExitEarly(ref1Idx, ref2Idx);
}

void QueryEvaluator::combineOneSyn(Result result, int refIdx, int otherRefIdx,
                                   bool isSecondRef, vector<pair<int, string>> *toRemove) {
    // refIdx should not be INVALID_INDEX
    map<VALUE, VALUE_SET> res;

    if (isSecondRef) {
        res = result.getResultList2();
    } else {
        res = result.getResultList1();
    }

    if (referenceAppearInClauses[refIdx]) {
        for (auto &valueToValuesPair : res) {
            string val = valueToValuesPair.first;
            if (!resultTable.hasVal(refIdx, val)) {
                toRemove->push_back({refIdx, val});
            }
        }
        for (string val : resultTable.getValues(refIdx)) {
            if (res.find(val) == res.end()) {
                toRemove->push_back({refIdx, val});
            }
        }
    }
    referenceAppearInClauses[refIdx] = true;
    for (auto &valueToValuesPair : res) {
        if (otherRefIdx == INVALID_INDEX) {
            resultTable.addValue(refIdx, valueToValuesPair.first);
        } else {
            resultTable.addValueWithLink(refIdx, valueToValuesPair.first,
                                         otherRefIdx, valueToValuesPair.second);
        }
    }
}

void QueryEvaluator::combineTwoSyn(Result result, int ref1Idx, int ref2Idx) {
    // ref1Idx, ref2Idx should not be INVALID_INDEX
    map<VALUE, VALUE_SET> res1;
    map<VALUE, VALUE_SET> res2;
    vector<pair<int, string>> toRemove;

    res1 = result.getResultList1();
    res2 = result.getResultList2();

    // remove links in the intermediate res
    removeLinkIRes(res1, ref1Idx, ref2Idx);
    removeLinkIRes(res2, ref2Idx, ref1Idx);

    // remove links in the result table
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
                resultTable.removeLink(ref1Idx, sourceVal, ref2Idx, targetVal);
            }
        }
    }
    // add all
    for (auto &valueToVals : res1) {
        resultTable.addValueWithLink(ref1Idx, valueToVals.first, ref2Idx,
                                     valueToVals.second);
    }
    for (auto &valueToVals : res2) {
        resultTable.addValueWithLink(ref2Idx, valueToVals.first, ref1Idx,
                                     valueToVals.second);
    }

    // remove those with no link in the resultTable
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
}

void QueryEvaluator::removeLinkIRes(map<VALUE, VALUE_SET> &iRes, int thisIdx,
                                    int otherIdx) {
    for (auto it = iRes.begin(); it != iRes.end();) {
        string sourceVal = it->first;
        VALUE_SET vals = it->second;
        bool erased = false;
        for (string targetVal : vals) {
            if (!resultTable.hasLinkBetweenValues(thisIdx, sourceVal, otherIdx,
                                                  targetVal)) {
                iRes[sourceVal].erase(targetVal);
                if (iRes[sourceVal].size() == 0) {
                    it = iRes.erase(it);
                    erased = true;
                }
            }
        }
        if (!erased) {
            ++it;
        }
    }
}

int QueryEvaluator::getRefIndex(Reference *ref) {
    int index = INVALID_INDEX;
    for (int i = 0; i < references.size(); i++) {
        if (references[i]->equals(*ref)) {
            index = i;
        }
    }
    return index;
}

bool QueryEvaluator::canExitEarly(int idx1, int idx2) {
    if (idx1 != INVALID_INDEX && resultTable.isColumnEmpty(idx1)) {
        return true;
    }

    if (idx2 != INVALID_INDEX && resultTable.isColumnEmpty(idx2)) {
        return true;
    }
    return false;
}

vector<vector<string>> QueryEvaluator::handleAttr(vector<vector<string>> input) {
    vector<vector<string>> updatedResList;
    for (auto res : input) {
        if (returnRefs.size() != res.size()) {
            throw invalid_argument(
                "input res and returnRefs have different size");
        }

        int size = returnRefs.size();
        vector<string> updatedRes(size);
        for (int i = 0; i < size; i++) {
            string updatedValue = res[i];
            Reference *ref = returnRefs[i];
            DesignEntityType desType = ref->getDeType();
            ReferenceAttribute attr = ref->getAttr();

            if (attr != ReferenceAttribute::NAME) {
                updatedRes[i] = updatedValue;
                continue;
            }

            if (desType == DesignEntityType::CALL) {
                updatedValue = pkb->getCallProcedure(stoi(updatedValue));
            } else if (desType == DesignEntityType::PRINT) {
                updatedValue = pkb->getPrintVariable(stoi(updatedValue));
            } else if (desType == DesignEntityType::READ) {
                updatedValue = pkb->getReadVariable(stoi(updatedValue));
            }

            updatedRes[i] = updatedValue;
        }
        updatedResList.push_back(updatedRes);
    }
    return updatedResList;
}
