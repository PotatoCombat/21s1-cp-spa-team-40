#pragma once

#include <set>
#include <string>

#include "common/model/Statement.h"

#include "pkb/PKB.h"

#include "query_processor/exception/ClauseHandlerError.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/model/Result.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

/**
 * Represents a handler with hidden internal logic and the ability to be
 * evaluated
 */
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
    static set<DesignEntityType> ASSIGN_STMT_DES_SET;

    static set<ReferenceType> ALL_VALID_REF;
    static set<ReferenceType> NO_WILDCARD_REF;

    /**
     * Creates a handler to be evaluated
     * @param clause pointer to a clause representing a relationship
     * @param pkb pointer to a pkb object containing information about the
     * source code
     * @param validClauseType the clause type that this handler supports
     */
    ClauseHandler(Clause *clause, PKB *pkb, ClauseType validClauseType);

    /**
    * Gets all r1 values such that the relationship between r1 and r2 holds
    * Can be overriden to handle different clause types
    * @param r2
    * @return all valid r1 values
    */
    virtual set<string> getR1ClauseR2(string r2) = 0;

    /**
     * Gets all r2 values such that the relationship between r1 and r2 holds
     * Can be overriden to handle different clause types
     * @param r1
     * @return all valid r2 values
     */
    virtual set<string> getR2ClausedR1(string r1) = 0;

    /**
    * Checks is the relationship between r1 and r2 holds
    * Can be overriden to handle different clause types
    * @param r1
    * @param r2
    * @return true if the relationship holds, false otherwise
    */
    virtual bool isR1ClauseR2(string r1, string r2) = 0;

    /**
    * Ensures that the information in the clause if valid
    */
    virtual void validate();

    bool isType(string val, DesignEntityType type);

    /**
    * Handles cases where both references are wildcards
    */
    Result evalWcWc();

    /**
     * Handles cases where both references are constants
     */
    Result evalConstConst();

    /**
     * Handles cases where the first references is a constant, the second
     * reference is a wildcard
     */
    Result evalConstWc();

    /**
     * Handles cases where the first references is a wildcard, the second
     * reference is a constant
     */
    Result evalWcConst();

    /**
     * Handles cases where the first references is a synonym, the second
     * reference is a constant
     */
    Result evalSynConst();

    /**
     * Handles cases where the first references is a constant, the second
     * reference is a synonym
     */
    Result evalConstSyn();

    /**
    * Handles cases where neither of the references is constant
    */
    Result evalNotConstNotConst();
    
    /**
    * Handles cases where both references represent to the same synonym 
    */
    Result evalSameSyn();

    void setResultListForOneRef(Result &result, Reference *thisRef, Reference *otherRef, bool isFirstRef);

public:
    /**
    * Evaluates the handler based on the clause's type and the references' types
    * @return a result object
    */ 
    virtual Result eval();

    /**
    * Gets all values of the reference from the pkb
    * @param pkb pointer to a pkb object
    * @param ref a reference object
    */
    static set<string> getAll(PKB* pkb, Reference ref);
};
