#include "query_processor/relationship_handler/ClauseHandler.h"

set<DesignEntityType> ClauseHandler::STMT_DES_SET {
    DesignEntityType::STMT,
    DesignEntityType::ASSIGN,
    DesignEntityType::CALL,
    DesignEntityType::IF,
    DesignEntityType::PRINT,
    DesignEntityType::READ,
    DesignEntityType::WHILE
};

set<DesignEntityType> ClauseHandler::PROCEDURE_DES_SET {
    DesignEntityType::PROCEDURE
};

set<DesignEntityType> ClauseHandler::VARIABLE_DES_SET {
    DesignEntityType::VARIABLE
};

set<DesignEntityType> ClauseHandler::ALL_DES_SET{
    DesignEntityType::STMT,    DesignEntityType::ASSIGN,
    DesignEntityType::CALL,    DesignEntityType::IF,
    DesignEntityType::PRINT,   DesignEntityType::READ,
    DesignEntityType::WHILE,   DesignEntityType::PROCEDURE,
    DesignEntityType::VARIABLE};

set<DesignEntityType> ClauseHandler::ASSIGN_STMT_DES_SET {
    DesignEntityType::STMT,
    DesignEntityType::ASSIGN,
};

set<ReferenceType> ClauseHandler::ALL_VALID_REF {
    ReferenceType::CONSTANT, ReferenceType::SYNONYM, ReferenceType::WILDCARD
};

set<ReferenceType> ClauseHandler::NO_WILDCARD_REF{
    ReferenceType::CONSTANT, ReferenceType::SYNONYM
};

/**
 * Creates a handler to be evaluated
 * @param clause pointer to a clause representing a relationship
 * @param pkb pointer to a pkb object containing information about the
 * source code
 * @param validClauseType the clause type that this handler supports
 */
ClauseHandler::ClauseHandler(Clause *clause, PKB *pkb, ResultCache *cache,
                             ClauseType validClauseType)
    : clause(clause), pkb(pkb), cache(cache), validClauseType(validClauseType) {}

/**
 * Evaluates the handler based on the clause's type and the references' types
 * @return a result object
 */
Result ClauseHandler::eval() {
    ReferenceType refType1 = clause->getFirstReference()->getRefType();
    ReferenceType refType2 = clause->getSecondReference()->getRefType();

    validate();
  
    // WILDCARD WILDCARD
    if (refType1 == ReferenceType::WILDCARD &&
        refType2 == ReferenceType::WILDCARD) {
        return evalWcWc();
    }

    // CONSTANT CONSTANT
    if (refType1 == ReferenceType::CONSTANT &&
        refType2 == ReferenceType::CONSTANT) {
        return evalConstConst();
    }

    // CONSTANT WILDCARD
    if (refType1 == ReferenceType::CONSTANT &&
        refType2 == ReferenceType::WILDCARD) {
        return evalConstWc();
    }

    // WILDCARD CONSTANT
    if (refType1 == ReferenceType::WILDCARD &&
        refType2 == ReferenceType::CONSTANT) {
        return evalWcConst();
    }

    // SYNONYM CONSTANT
    if (refType1 == ReferenceType::SYNONYM &&
        refType2 == ReferenceType::CONSTANT) {
        return evalSynConst();
    }

    // CONSTANT SYNONYM
    if (refType1 == ReferenceType::CONSTANT &&
        refType2 == ReferenceType::SYNONYM) {
        return evalConstSyn();
    }

    // NEITHER IS CONSTANT
    return evalNotConstNotConst();
}

/**
 * Handles cases where both references are wildcards
 */
Result ClauseHandler::evalWcWc() {
    Result result;
    Reference *ref1 = clause->getFirstReference();
    set<string> res1s = getAll(pkb, *ref1);
    for (auto res1 : res1s) {
        if (getR2ClausedR1Wrapper(res1).empty()) {
            continue;
        }
        result.setValid(true);
        return result;
    }
    result.setValid(false);
    return result;
}

/**
 * Handles cases where both references are constants
 */
Result ClauseHandler::evalConstConst() {
    Result result;
    string val1 = clause->getFirstReference()->getValue();
    string val2 = clause->getSecondReference()->getValue();
    result.setValid(isR1ClauseR2Wrapper(val1, val2));
    return result;
}

/**
 * Handles cases where the first references is a constant, the second
 * reference is a wildcard
 */
Result ClauseHandler::evalConstWc() {
    Result result;
    string val1 = clause->getFirstReference()->getValue();
    result.setValid(!getR2ClausedR1Wrapper(val1).empty());
    return result;
}

/**
 * Handles cases where the first references is a wildcard, the second
 * reference is a constant
 */
Result ClauseHandler::evalWcConst() {
    Result result;
    string val2 = clause->getSecondReference()->getValue();
    result.setValid(!getR1ClauseR2Wrapper(val2).empty());
    return result;
}

/**
 * Handles cases where the first references is a synonym, the second
 * reference is a constant
 */
Result ClauseHandler::evalSynConst() {
    Result result;
    Reference *ref1 = clause->getFirstReference();
    string val2 = clause->getSecondReference()->getValue();
    map<VALUE, VALUE_SET> firstStmtResults;
    set<string> res1s = getR1ClauseR2Wrapper(val2);
    for (string res1 : res1s) {
        if (!isType(res1, ref1->getDeType())) {
            continue;
        }
        firstStmtResults[res1] = VALUE_SET{};
    }
    result.setResultList1(ref1, firstStmtResults);
    return result;
}

/**
 * Handles cases where the first references is a constant, the second
 * reference is a synonym
 */
Result ClauseHandler::evalConstSyn() {
    Result result;
    Reference *ref2 = clause->getSecondReference();
    string val1 = clause->getFirstReference()->getValue();
    map<VALUE, VALUE_SET> secondStmtResults;
    set<string> res2s = getR2ClausedR1Wrapper(val1);
    for (auto res2 : res2s) {
        if (!isType(res2, ref2->getDeType())) {
            continue;
        }
        secondStmtResults[res2] = VALUE_SET{};
    }
    result.setResultList2(ref2, secondStmtResults);
    return result;
}

/**
 * Handles cases where neither of the references is constant
 */
Result ClauseHandler::evalNotConstNotConst() {
    Result result;
    Reference *ref1 = clause->getFirstReference();
    Reference *ref2 = clause->getSecondReference();
    ReferenceType refType1 = ref1->getRefType();
    ReferenceType refType2 = ref2->getRefType();
    string val1 = ref1->getValue();
    string val2 = ref2->getValue();

    // same synonym
    if (refType1 == ReferenceType::SYNONYM &&
        refType2 == ReferenceType::SYNONYM && val1 == val2) {
        return evalSameSyn();
    }

    // if first arg is SYNONYM
    if (refType1 == ReferenceType::SYNONYM) {
        setResultListForOneRef(result, ref1, ref2, true);
    }

    // if second arg is SYNONYM
    if (refType2 == ReferenceType::SYNONYM) {
        setResultListForOneRef(result, ref2, ref1, false);
    }

    cache->setFullyCachedAllValues(clause->getType());
    return result;
}

/**
 * Handles cases where both references represent the same synonym
 */
Result ClauseHandler::evalSameSyn() {
    Result result;
    map<VALUE, VALUE_SET> resultList;
    Reference *ref = clause->getFirstReference();
    set<string> res1s = getAll(pkb, *ref);
    for (auto res1 : res1s) {
        if (!isR1ClauseR2Wrapper(res1, res1)) {
            continue;
        }
        resultList[res1] = VALUE_SET{};
    }
    result.setResultList1(ref, resultList);
    result.setResultList2(ref, resultList);
    return result;
}

void ClauseHandler::setResultListForOneRef(Result &result, Reference *thisRef,
                                            Reference *otherRef,
                                            bool isFirstRef) {
    set<string> (ClauseHandler::*getOtherRefValues)(string);
    void (Result::*setThisResultList)(Reference *, map<VALUE, VALUE_SET>);
    if (isFirstRef) {
        getOtherRefValues = &ClauseHandler::getR2ClausedR1Wrapper;
        setThisResultList = &Result::setResultList1;
    } else {
        getOtherRefValues = &ClauseHandler::getR1ClauseR2Wrapper;
        setThisResultList = &Result::setResultList2;
    }

    map<VALUE, VALUE_SET> thisStmtResults;
    set<string> thisVals = getAll(pkb, *thisRef);

    for (string thisVal : thisVals) {
        set<string> otherVals = (this->*getOtherRefValues)(thisVal);
        VALUE_SET related;
        bool valid = false;
        for (string otherVal : otherVals) {
            if (!isType(otherVal, otherRef->getDeType())) {
                continue;
            }
            valid = true;
            related.insert(otherVal);
        }
        if (valid) {
            thisStmtResults[thisVal] = related;
        }
    }

    (result.*setThisResultList)(thisRef, thisStmtResults);
}

/**
 * Ensures that the information in the clause if valid
 */
void ClauseHandler::validate() {
    Reference *ref1 = clause->getFirstReference();
    Reference *ref2 = clause->getSecondReference();
    if (validDesType1->find(ref1->getDeType()) == validDesType1->end()) {
        throw ClauseHandlerError("invalid design entity type");
    }
    if (validDesType2->find(ref2->getDeType()) == validDesType2->end()) {
        throw ClauseHandlerError("invalid design entity type");
    }
    if (validRefType1->find(ref1->getRefType()) == validRefType1->end()) {
        throw ClauseHandlerError("invalid reference type");
    }
    if (validRefType2->find(ref2->getRefType()) == validRefType2->end()) {
        throw ClauseHandlerError("invalid reference type");
    }
    if (clause->getType() != validClauseType) {
        throw ClauseHandlerError("invalid clause type");
    }
}

/**
 * Gets all values of the reference from the pkb
 * @param pkb pointer to a pkb object
 * @param ref a reference object
 */
set<string> ClauseHandler::getAll(PKB* pkb, Reference ref) {
    set<string> res;
    DesignEntityType desType = ref.getDeType();
    switch (desType) { 
    case DesignEntityType::CONSTANT:
        for (string s : pkb->getAllConsts().asVector()) {
            res.insert(s);
        }
        return res;
    case DesignEntityType::PROCEDURE:
        for (string s : pkb->getAllProcs().asVector()) {
            res.insert(s);
        }
        return res;
    case DesignEntityType::VARIABLE:
        for (string s : pkb->getAllVars().asVector()) {
            res.insert(s);
        }
        return res;
    case DesignEntityType::STMT: 
        for (int i : pkb->getAllStmts().asVector()) {
            res.insert(to_string(i));
        }
        return res;
    default:
        StatementType type = DesignEntityTypeHelper::desTypeToStmtType(desType);
        for (int i : pkb->getAllStmts(type).asVector()) {
            res.insert(to_string(i));
        }
        return res;
    }
}

bool ClauseHandler::isType(string val, DesignEntityType type) {
    bool isNumber = !val.empty() && find_if(val.begin(), 
        val.end(), [](unsigned char c) { return !isdigit(c); }) == val.end();

    if (!isNumber) {
        return true;
    }

    if (type == DesignEntityType::STMT || type == DesignEntityType::CONSTANT) {
        return true;
    }

    StatementType stmtType = pkb->getStmtType(stoi(val));
    return DesignEntityTypeHelper::isDesTypeStmtType(type, stmtType);
}

set<string> ClauseHandler::getR1ClauseR2Wrapper(string r2) {
    ClauseType clsType = clause->getType();
    if (useCache && cache->isR2FullyCached(r2, clsType) &&
        clsType != ClauseType::WITH) {
        return cache->getR1Values(r2, clsType);
    }

    set<string> res = getR1ClauseR2(r2);
    for (auto r1 : res) {
        cache->cache(r1, r2, clsType);
    }
    cache->setR2FullyCached(r2, clsType);
    return res;
}

set<string> ClauseHandler::getR2ClausedR1Wrapper(string r1) {
    ClauseType clsType = clause->getType();
    if (useCache && cache->isR1FullyCached(r1, clsType) &&
        clsType != ClauseType::WITH) {
        return cache->getR2Values(r1, clsType);
    }

    set<string> res = getR2ClausedR1(r1);
    for (auto r2 : res) {
        cache->cache(r1, r2, clsType);
    }
    cache->setR1FullyCached(r1, clsType);
    return res;
}

bool ClauseHandler::isR1ClauseR2Wrapper(string r1, string r2) {
    ClauseType clsType = clause->getType();
    if (useCache && cache->isR1FullyCached(r1, clsType) &&
        clsType != ClauseType::WITH) {
        return cache->isR1ClauseR2(r1, r2, clsType);
    }
    bool valid = isR1ClauseR2(r1, r2);
    if (valid) {
        cache->cache(r1, r2, clsType);
    }

    return valid;
}
