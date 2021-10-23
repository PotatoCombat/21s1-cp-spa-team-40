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
#include "query_processor/model/Query.h"
#include "query_processor/model/Reference.h"

#include "query_processor/relationship_handler/PatternHandler.h"
#include "query_processor/relationship_handler/CallsHandler.h"
#include "query_processor/relationship_handler/CallsStarHandler.h"
#include "query_processor/relationship_handler/ClauseHandler.h"
#include "query_processor/relationship_handler/ClauseHandlerError.h"
#include "query_processor/relationship_handler/FollowsHandler.h"
#include "query_processor/relationship_handler/FollowsStarHandler.h"
#include "query_processor/relationship_handler/ModifiesProcHandler.h"
#include "query_processor/relationship_handler/ModifiesStmtHandler.h"
#include "query_processor/relationship_handler/NextHandler.h"
#include "query_processor/relationship_handler/NextStarHandler.h"
#include "query_processor/relationship_handler/ParentHandler.h"
#include "query_processor/relationship_handler/ParentStarHandler.h"
#include "query_processor/relationship_handler/UsesProcHandler.h"
#include "query_processor/relationship_handler/UsesStmtHandler.h"
#include "query_processor/relationship_handler/WithHandler.h"
#include "query_processor/relationship_handler/AffectsHandler.h"

using namespace std;

class QueryEvaluator {
private:
    static const int INVALID_INDEX = -1;

    PKB *pkb;
    vector<Reference *> returnRefs;
    vector<Reference *> references;
    vector<Clause *> clauses;
    vector<bool> referenceAppearInClauses;
    ResultTable resultTable;
    vector<vector<bool>> appearInSameClauseAlr;

    void clear();

    Result getTempResult(Clause *clause);

    void evalClauses(bool &exitEarly);

    vector<string> finaliseResult(bool exitEarly = false);

    void combineResult(Result result, int ref1Index, int ref2Index, bool &exitEarly);

    void combineOneSyn(Result result, int refIdx, int otherRefIdx,
                       bool isSecondRef, vector<pair<int, string>> *toRemove);

    void combineTwoSyn(Result result, int ref1Idx, int ref2Idx);

    int getRefIndex(Reference *ref);

    // Decides if the program can exit early
    // based on the state of the current resultTable at idx1 and idx2
    bool canExitEarly(int idx1, int idx2);

    // Convert return results based on the return ref's attribute
    vector<vector<string>> handleAttr(vector<vector<string>> input);

public:
    QueryEvaluator(PKB *pkb);
    vector<string> evaluateQuery(Query query);
};
