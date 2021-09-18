#include "query_processor/QueryEvaluator.h"

void QueryEvaluator::clear() {
    resultTable.clear();
    references.clear();
    clauses.clear();
    patterns.clear();
    referenceAppearInClauses.clear();
    allQueriesReturnTrue = true;
    returnReference = NULL;
}

QueryEvaluator::QueryEvaluator(PKB *pkb)
    : pkb(pkb), allQueriesReturnTrue(true), returnReference(NULL) {}

vector<string> QueryEvaluator::evaluateQuery(Query query) {
    try {
        clear();
        returnReference = query.getReturnReference();
        references = query.getReferences();
        clauses = query.getClauses();
        patterns = query.getPatterns();
        resultTable.init(references.size());
        vector<bool> referenceAppearInClauses(references.size(), false);
        this->referenceAppearInClauses = referenceAppearInClauses;
        bool allQueriesReturnTrue = true;

        evalPattern();

        evalSuchThat();

        return finaliseResult();

    } catch (ClauseHandlerError &e) {
        // to be implemented later
        return vector<string>{};
    }
}

void QueryEvaluator::evalPattern() {
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
        tempResult = patternHandler->eval(ref1Index, ref2Index);
        combineResult(tempResult);
    }
}

void QueryEvaluator::evalSuchThat() {
    // handle clauses
    for (Clause *clause : clauses) {
        Result tempResult;

        ClauseHandler *clauseHandler;

        // TODO: add more handlers for clauseType later
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
        tempResult = clauseHandler->eval(ref1Index, ref2Index);
        combineResult(tempResult);
    }
}

vector<string> QueryEvaluator::finaliseResult() {
    // returns empty result if one of the boolean clauses returns false
    if (!allQueriesReturnTrue) {
        return vector<string>();
    }

    // returns empty result if one of the references has no matching result
    for (int i = 0; i < references.size(); i++) {
        if (referenceAppearInClauses[i] && resultTable.getValues(i).empty()) {
            return vector<string>();
        }
    }

    int resultIndex = -1;
    for (int i = 0; i < references.size(); i++) {
        if (references[i]->equals(*returnReference)) {
            resultIndex = i;
        }
    }

    if (!referenceAppearInClauses[resultIndex]) {
        vector<string> result;

        if (returnReference->getDeType() == DesignEntityType::PROCEDURE) {
            result = pkb->getAllProcs().asVector();
            return result;
        }
        if (returnReference->getDeType() == DesignEntityType::VARIABLE) {
            result = pkb->getAllVars().asVector();
            return result;
        }
        if (returnReference->getDeType() == DesignEntityType::CONSTANT) {
            result = pkb->getAllConsts().asVector();
            return result;
        }
        if (returnReference->getDeType() == DesignEntityType::ASSIGN) {
            vector<StmtIndex> stmts =
                pkb->getAllStmts(StatementType::ASSIGN).asVector();
            toString(stmts, result);
            return result;
        }
        if (returnReference->getDeType() == DesignEntityType::CALL) {
            vector<StmtIndex> stmts =
                pkb->getAllStmts(StatementType::CALL).asVector();
            toString(stmts, result);
            toString(stmts, result);
            return result;
        }
        if (returnReference->getDeType() == DesignEntityType::IF) {
            vector<StmtIndex> stmts =
                pkb->getAllStmts(StatementType::IF).asVector();
            toString(stmts, result);
            return result;
        }
        if (returnReference->getDeType() == DesignEntityType::WHILE) {
            vector<StmtIndex> stmts =
                pkb->getAllStmts(StatementType::WHILE).asVector();
            toString(stmts, result);
            return result;
        }
        if (returnReference->getDeType() == DesignEntityType::PRINT) {
            vector<StmtIndex> stmts =
                pkb->getAllStmts(StatementType::PRINT).asVector();
            toString(stmts, result);
            return result;
        }
        if (returnReference->getDeType() == DesignEntityType::READ) {
            vector<StmtIndex> stmts =
                pkb->getAllStmts(StatementType::READ).asVector();
            toString(stmts, result);
            return result;
        }
        vector<StmtIndex> stmts = pkb->getAllStmts().asVector();
        toString(stmts, result);

        return result;
    }

    return resultTable.getValues(resultIndex);
}

void QueryEvaluator::combineResult(Result result) {
    allQueriesReturnTrue = allQueriesReturnTrue && result.isResultValid();

    Reference *ref1 = NULL, *ref2 = NULL;
    vector<ValueToPointersMap> res1;
    vector<ValueToPointersMap> res2;
    vector<pair<int, string>> toRemoveLater;

    if (result.hasResultList1() && result.hasResultList2()) {
        ref1 = result.getReference1();
        int refIndex1 = getRefIndex(ref1);
        ref2 = result.getReference2();
        int refIndex2 = getRefIndex(ref2);
        if (referenceAppearInClauses[refIndex1] && referenceAppearInClauses[refIndex2]) {
            res1 = result.getResultList1();
            res2 = result.getResultList2();
            // remove links
            for (ValueToPointersMap &map : res1) {
                for (auto pointer : map.getPointers()) {
                    if (!resultTable.hasLink(refIndex1, map.getValue(), 
                                             pointer.first, pointer.second)) {
                        map.erasePointer(pointer);
                    }
                } 
            }
            for (ValueToPointersMap &map : res2) {
                for (auto pointer : map.getPointers()) {
                    if (!resultTable.hasLink(refIndex2, map.getValue(),
                                             pointer.first, pointer.second)) {
                        map.erasePointer(pointer);
                    }
                }
            }
            map<string, set<pair<int, string>>> m;
            for (ValueToPointersMap map : res1) {
                m[map.getValue()] = map.getPointers();
            }

            vector<string> existingValues = resultTable.getValues(refIndex1);
            for (auto value : existingValues) {
                for (auto pointer : resultTable.getPointers(refIndex1, value)) {
                    if (m[value].find(pointer) == m[value].end()) {
                        resultTable.removeLink(refIndex1, value, pointer.first, pointer.second);
                    }
                }
            }
            // add all
            resultTable.addValue(refIndex1, res1);
            resultTable.addValue(refIndex2, res2);
            // remove those with no link
            for (string refValue : resultTable.getValues(refIndex1)) {
                if (!resultTable.hasPointerToRef(refIndex1, refValue, refIndex2)) {
                    resultTable.removeMap(refIndex1, refValue);
                }
            }
            for (string refValue : resultTable.getValues(refIndex2)) {
                if (!resultTable.hasPointerToRef(refIndex2, refValue, refIndex1)) {
                    resultTable.removeMap(refIndex2, refValue);
                }
            }
            return;
        }
    }

    if (result.hasResultList1()) {
        res1 = result.getResultList1();
        ref1 = result.getReference1();
        int refIndex1 = getRefIndex(ref1);
        // create sets
        set<string> refsInTable;
        set<string> refsInResult;
        for (string refInTable : resultTable.getValues(refIndex1)) {
            refsInTable.insert(refInTable);
        }
        for (ValueToPointersMap map : res1) {
            refsInResult.insert(map.getValue());
        }
        if (referenceAppearInClauses[refIndex1]) {
            for (auto ref : refsInTable) {
                if (refsInResult.find(ref) == refsInResult.end()) {
                    toRemoveLater.push_back(make_pair(refIndex1, ref));
                }
            }
            for (auto ref : refsInResult) {
                if (refsInTable.find(ref) == refsInTable.end()) {
                    toRemoveLater.push_back(make_pair(refIndex1, ref));
                }
            }
        }
        referenceAppearInClauses[refIndex1] = true;
        resultTable.addValue(refIndex1, res1);
    }

    if (result.hasResultList2()) {
        res2 = result.getResultList2();
        ref2 = result.getReference2();
        int refIndex2 = getRefIndex(ref2);
        // create sets
        set<string> refsInTable;
        set<string> refsInResult;
        for (string refInTable : resultTable.getValues(refIndex2)) {
            refsInTable.insert(refInTable);
        }
        for (ValueToPointersMap map : res2) {
            refsInResult.insert(map.getValue());
        }
        if (referenceAppearInClauses[refIndex2]) {
            for (auto ref : refsInTable) {
                if (refsInResult.find(ref) == refsInResult.end()) {
                    toRemoveLater.push_back(make_pair(refIndex2, ref));
                }
            }
            for (auto ref : refsInResult) {
                if (refsInTable.find(ref) == refsInTable.end()) {
                    toRemoveLater.push_back(make_pair(refIndex2, ref));
                }
            }
        }
        referenceAppearInClauses[refIndex2] = true;
        resultTable.addValue(refIndex2, res2);
    }

    for (auto mapCoord : toRemoveLater) {
        resultTable.removeMap(mapCoord.first, mapCoord.second);
    }
}

void QueryEvaluator::toString(vector<int> &vectorIn,
                              vector<string> &vectorOut) {
    transform(vectorIn.begin(), vectorIn.end(), back_inserter(vectorOut),
              [](int i) { return std::to_string(i); });
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
