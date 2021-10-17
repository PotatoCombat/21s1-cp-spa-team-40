#pragma once

#include <string>
#include <set>
#include <vector>

#include "pkb/PKB.h"

#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/relationship_handler/ClauseHandlerError.h"
#include "query_processor/relationship_handler/ModifiesStmtHandler.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

using namespace std;

class PatternHandler {
private:
    Clause *patternClause;
    PKB *pkb;
    void validate();

    // Wrappers for PKB functions
    set<int> getPartialAssignPatternStmts(string var, vector<string> pattern);
    set<int> getExactAssignPatternStmts(string var, vector<string> pattern);

    set<int> getIfPatternStmts(string var, vector<string> pattern);
    set<int> getWhilePatternStmts(string var, vector<string> pattern);

    bool partialAssignPattern(int stmt, string var, vector<string> pattern);
    bool exactAssignPattern(int stmt, string var, vector<string> pattern);

    bool ifPattern(int stmt, string var, vector<string> pattern);
    bool whilePattern(int stmt, string var, vector<string> pattern);

public:
    PatternHandler(Clause *patternClause, PKB *pkb)
        : patternClause(patternClause), pkb(pkb) {}
    Result eval();
};
