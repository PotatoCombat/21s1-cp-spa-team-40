#pragma once

#include <set>
#include <string>
#include <vector>

#include "pkb/PKB.h"

#include "query_processor/exception/ClauseHandlerError.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

#include "query_processor/relationship_handler/ClauseHandler.h"
#include "query_processor/relationship_handler/ModifiesStmtHandler.h"

using namespace std;

class PatternHandler;
// typedef function pointers declared after the declaration of the class
typedef set<int> (PatternHandler::*GetPatternStmtsFunc)(string, vector<string>);
typedef bool (PatternHandler::*HasPatternFunc)(int, string, vector<string>);

class PatternHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;

    void validate() override;

    // Wrappers for PKB functions
    set<int> getPartialAssignPatternStmts(string var, vector<string> pattern);
    set<int> getExactAssignPatternStmts(string var, vector<string> pattern);

    set<int> getIfPatternStmts(string var, vector<string> pattern);
    set<int> getWhilePatternStmts(string var, vector<string> pattern);

    bool partialAssignPattern(int stmt, string var, vector<string> pattern);
    bool exactAssignPattern(int stmt, string var, vector<string> pattern);

    bool ifPattern(int stmt, string var, vector<string> pattern);
    bool whilePattern(int stmt, string var, vector<string> pattern);

    void getFunctions(GetPatternStmtsFunc &getPatternStmts,
                      HasPatternFunc &hasPattern);

public:
    PatternHandler(Clause *patternClause, PKB *pkb);
};
