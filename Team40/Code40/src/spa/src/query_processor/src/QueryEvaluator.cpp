#include "query_processor/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB* pkb) { this->pkb = pkb; }

vector<string> QueryEvaluator::evaluateQuery(Query query) {
    try {
        Reference *returnReference = query.getReturnReference();
        vector<Reference *> references = query.getReferences();
        vector<Clause *> clauses = query.getClauses();
        vector<PatternClause *> patterns = query.getPatterns();
        vector<vector<string>> results(references.size(), vector<string>());
        vector<bool> referenceAppearInClauses(references.size(), false);
        bool allQueriesReturnTrue = true;

        evalPattern(results, references, patterns, referenceAppearInClauses, allQueriesReturnTrue);

        evalSuchThat(results, references, clauses, referenceAppearInClauses, allQueriesReturnTrue);

        return finaliseResult(returnReference, results, references, referenceAppearInClauses, allQueriesReturnTrue);
        
    } catch (ClauseHandlerError &e) {
        // to be implemented later
        return vector<string>{};
    }
}

void QueryEvaluator::evalPattern(vector<vector<string>>& results, vector<Reference*>& references,
    vector<PatternClause*>& patterns, vector<bool>& referenceAppearInClauses, bool& allQueriesReturnTrue) {
    for (PatternClause *pattern : patterns) {
        Result tempResult;
        AssignPatternHandler* patternHandler;

        if (pattern->getStmt()->getDeType() == DesignEntityType::ASSIGN) {
            AssignPatternHandler handler(pattern, pkb);
            patternHandler = &handler;
        }

        // eval and combine result
        tempResult = patternHandler->eval();
        allQueriesReturnTrue =
            allQueriesReturnTrue && tempResult.isResultValid();

        if (tempResult.hasResultList1()) {
            combineResult(results, references, tempResult.getResultList1(),
                          tempResult.getReference1(), referenceAppearInClauses);
        }

        if (tempResult.hasResultList2()) {
            combineResult(results, references, tempResult.getResultList2(),
                          tempResult.getReference2(), referenceAppearInClauses);
        }
    }
 }

void QueryEvaluator::evalSuchThat(vector<vector<string>>& results, vector<Reference*>& references,
    vector<Clause*>& clauses, vector<bool>& referenceAppearInClauses, bool& allQueriesReturnTrue) {
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

        // eval and combine result
        tempResult = clauseHandler->eval();
        allQueriesReturnTrue =
            allQueriesReturnTrue && tempResult.isResultValid();

        if (tempResult.hasResultList1()) {
            combineResult(results, references, tempResult.getResultList1(),
                          tempResult.getReference1(), referenceAppearInClauses);
        }

        if (tempResult.hasResultList2()) {
            combineResult(results, references, tempResult.getResultList2(),
                          tempResult.getReference2(), referenceAppearInClauses);
        }
    }
}

vector<string> QueryEvaluator::finaliseResult(Reference* returnReference, vector<vector<string>>& results, vector<Reference*>& references,
    vector<bool>& referenceAppearInClauses, bool& allQueriesReturnTrue) {
    // returns empty result if one of the boolean clauses returns false
    if (!allQueriesReturnTrue) {
        return vector<string>();
    }

    // returns empty result if one of the references has no matching result
    for (int i = 0; i < references.size(); i++) {
        if (referenceAppearInClauses[i] && results[i].empty()) {
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
            vector<int> consts = pkb->getAllConsts().asVector();
            toString(consts, result);
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

    return results[resultIndex];
}

void QueryEvaluator::combineResult(vector<vector<string>> &results, vector<Reference *> &references,
                                   vector<string> result, Reference* referenceToCombine, vector<bool> &referenceAppearInClauses) {
    int index = -1;
    for (int i = 0; i < references.size(); i++) {
        if (references[i]->equals(*referenceToCombine)) {
            index = i;
        }
    }

    if (referenceAppearInClauses[index] == false) {
        results[index] = result;
        referenceAppearInClauses[index] = true;
    } else {
        vector<string> filteredResult;
        for (string element : results[index]) {
            if (find(result.begin(), result.end(), element) != result.end()) {
                filteredResult.push_back(element);
            }
        }
        results[index] = filteredResult;
    }
}

void QueryEvaluator::toString(vector<int> &vectorIn, vector<string> &vectorOut) {
    transform(vectorIn.begin(), vectorIn.end(), back_inserter(vectorOut),
              [](int i) { return std::to_string(i); });
}
