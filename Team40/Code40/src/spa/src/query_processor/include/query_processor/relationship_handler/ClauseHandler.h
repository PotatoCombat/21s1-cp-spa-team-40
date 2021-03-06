#pragma once

#include <set>
#include <string>

#include "common/model/Statement.h"

#include "pkb/PKB.h"

#include "query_processor/ResultCache.h"

#include "query_processor/exception/ClauseHandlerError.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"
#include "query_processor/model/Result.h"

/**
 * CommandPattern handler to evaluate query clauses
 */
class ClauseHandler {
protected:
    const int DUMMY_STMT_THRESHOLD = -2;

    Clause *clause;
    PKB *pkb;
    set<DesignEntityType> *validDesType1;
    set<DesignEntityType> *validDesType2;
    set<ReferenceType> *validRefType1;
    set<ReferenceType> *validRefType2;
    ClauseType validClauseType;

    // change this to false if don't want to use cache
    bool useCache = true;

    static set<DesignEntityType> STMT_DES_SET;
    static set<DesignEntityType> PROCEDURE_DES_SET;
    static set<DesignEntityType> VARIABLE_DES_SET;
    static set<DesignEntityType> ALL_DES_SET;
    static set<DesignEntityType> ASSIGN_STMT_DES_SET;

    static set<ReferenceType> ALL_VALID_REF;
    static set<ReferenceType> NO_WILDCARD_REF;

    ClauseHandler(Clause *clause, PKB *pkb, ClauseType validClauseType);

    set<string> getR1ClauseR2Wrapper(string r2);
    set<string> getR2ClausedR1Wrapper(string r1);
    bool isR1ClauseR2Wrapper(string r1, string r2);

    /**
     * Gets all reference 1 values such that the relationship between
     * reference 1 and reference 2 holds
     * Can be overriden to handle different clause types
     * @return all valid reference 1 values
     */
    virtual set<string> getR1ClauseR2(string r2) = 0;

    /**
     * Gets all reference 2 values such that the relationship between
     * reference 1 and reference 2 holds
     * Can be overriden to handle different clause types
     * @return all valid reference 2 values
     */
    virtual set<string> getR2ClausedR1(string r1) = 0;

    /**
     * Checks is the relationship between
     * reference 1 and reference 2 holds
     * Can be overriden to handle different clause types
     * @return true if the relationship holds, false otherwise
     */
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
    Result evalSameSyn();
    void setResultListForOneRef(Result &result, Reference *thisRef,
                                Reference *otherRef, bool isFirstRef);

    [[nodiscard]] inline bool isDummyStmt(ProgLineIndex progLineIndex) const {
        return progLineIndex <= DUMMY_STMT_THRESHOLD;
    }

public:
    virtual Result eval();
    static set<string> getAll(PKB *pkb, Reference ref);
};
