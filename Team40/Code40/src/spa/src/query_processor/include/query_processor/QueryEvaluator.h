#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "Result.h"

#include "pkb/PKB.h"

#include "query_processor/ResultFormatter.h"
#include "query_processor/ResultTable.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/PatternClause.h"
#include "query_processor/model/Query.h"
#include "query_processor/model/Reference.h"

#include "query_processor/relationship_handler/PatternHandler.h"
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

using namespace std;

class QueryEvaluator {
private:
    PKB *pkb;
    vector<Reference *> returnRefs;
    vector<Reference *> references;
    vector<Clause *> clauses;
    vector<PatternClause *> patterns;
    vector<bool> referenceAppearInClauses;
    ResultTable resultTable;
    bool allQueriesReturnTrue;

    void clear();

    void evalPattern();

    void evalSuchThat();

    vector<string> finaliseResult();

    void combineResult(Result result, int ref1Index, int ref2Index);

    int getRefIndex(Reference *ref);

public:
    QueryEvaluator(PKB *pkb);
    vector<string> evaluateQuery(Query query);
};
