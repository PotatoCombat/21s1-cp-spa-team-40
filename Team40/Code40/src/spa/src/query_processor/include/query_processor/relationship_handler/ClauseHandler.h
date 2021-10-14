#pragma once

#include <set>
#include <string>

#include "ClauseHandlerError.h"

#include "common/model/Statement.h"

#include "pkb/PKB.h"

#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

class ClauseHandler {
protected:
    Clause *clause;
    PKB *pkb;
    set<DesignEntityType> *validDesType1;
    set<DesignEntityType> *validDesType2;
    set<ReferenceType> *validRefType1;
    set<ReferenceType> *validRefType2;
    ClauseType validClauseType;

    static set<DesignEntityType> STMT_DES_SET;
    static set<DesignEntityType> PROCEDURE_DES_SET;
    static set<DesignEntityType> VARIABLE_DES_SET;
    static set<DesignEntityType> ALL_DES_SET;

    static set<ReferenceType> ALL_VALID_REF;
    static set<ReferenceType> NO_WILDCARD_REF;

    ClauseHandler(Clause *clause, PKB *pkb, ClauseType validClauseType);

    virtual set<string> getR1ClauseR2(string r2) = 0;
    virtual set<string> getR2ClausedR1(string r1) = 0;
    virtual bool isR1ClauseR2(string r1, string r2) = 0;

    virtual void validate();
    bool isType(string val, DesignEntityType type);

    Result evalWcWc();
    Result evalConstConst();
    Result evalConstWc();
    Result evalWcConst();
    Result evalSynConst();
    Result evalConstSyn();
    Result evalNotConstNotConst();

public:
    // evaluates the clause and writes the answer to the result object
    // Result invalid = false only happens when neither of the references is
    // synonym
    virtual Result eval();
    static set<string> getAll(PKB* pkb, Reference ref);
};
