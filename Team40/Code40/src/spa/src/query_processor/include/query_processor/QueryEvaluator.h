#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include "Result.h"

#include "pkb/PKB.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/Query.h"
#include "query_processor/model/Reference.h"
#include "query_processor/model/PatternClause.h"

#include "query_processor/relationship_handler/ClauseHandler.h"
#include "query_processor/relationship_handler/ClauseHandlerError.h"
#include "query_processor/relationship_handler/FollowsHandler.h"
#include "query_processor/relationship_handler/FollowsStarHandler.h"
#include "query_processor/relationship_handler/ModifiesProcHandler.h"
#include "query_processor/relationship_handler/ModifiesStmtHandler.h"
#include "query_processor/relationship_handler/ParentHandler.h"
#include "query_processor/relationship_handler/ParentStarHandler.h"
#include "query_processor/relationship_handler/UsesProcHandler.h"
#include "query_processor/relationship_handler/UsesStmtHandler.h"
#include "query_processor/relationship_handler/AssignPatternHandler.h"

using namespace std;

class QueryEvaluator {
private:
    PKB *pkb;

    void evalPattern(vector<vector<string>> &results, vector<Reference *> &references,
                        vector<PatternClause *> &patterns, vector<bool> &referenceAppearInClauses, bool &allQueriesReturnTrue);

    void evalSuchThat(vector<vector<string>> &results, vector<Reference *> &references,
                        vector<Clause *> &clauses, vector<bool> &referenceAppearInClauses, bool &allQueriesReturnTrue);

    vector<string> finaliseResult(Reference* returnReference, vector<vector<string>> &results, vector<Reference *> &references,
                        vector<bool> &referenceAppearInClauses, bool &allQueriesReturnTrue);

    void combineResult(vector<vector<string>> &results,
                       vector<Reference *> &references, vector<string> result,
                       Reference *referenceToCombine,
                       vector<bool> &referenceAppearInClauses);

    void toString(vector<int> &vectorIn, vector<string> &vectorOut);

public:
    QueryEvaluator(PKB *pkb);
    vector<string> evaluateQuery(Query query);
};
