#include "query_processor/QueryEvaluator.h"

void QueryEvaluator::clear() {
    results.clear();
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
        vector<vector<string>> results(references.size(), vector<string>());
        this->results = results;
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
        AssignPatternHandler* patternHandler;

        if (pattern->getStmt()->getDeType() == DesignEntityType::ASSIGN) {
            AssignPatternHandler handler(pattern, pkb);
            patternHandler = &handler;
        }

        // eval and combine result
        tempResult = patternHandler->eval();
        comebineResult(tempResult);
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

        // eval and combine result
        tempResult = clauseHandler->eval();
        comebineResult(tempResult);
        
    }
}

vector<string> QueryEvaluator::finaliseResult() {
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

void QueryEvaluator::combineVectorResult(vector<string> result, Reference* referenceToCombine) {
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

void QueryEvaluator::comebineResult(Result result) {
    allQueriesReturnTrue =
            allQueriesReturnTrue && result.isResultValid();

    if (result.hasResultList1()) {
        combineVectorResult(result.getResultList1(), result.getReference1());
    }

    if (result.hasResultList2()) {
        combineVectorResult(result.getResultList2(), result.getReference2());
    }
}

void QueryEvaluator::toString(vector<int> &vectorIn, vector<string> &vectorOut) {
    transform(vectorIn.begin(), vectorIn.end(), back_inserter(vectorOut),
              [](int i) { return std::to_string(i); });
}
