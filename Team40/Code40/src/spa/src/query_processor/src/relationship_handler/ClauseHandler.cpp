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

set<ReferenceType> ClauseHandler::ALL_VALID_REF {
    ReferenceType::CONSTANT, ReferenceType::SYNONYM, ReferenceType::WILDCARD
};

set<ReferenceType> ClauseHandler::NO_WILDCARD_REF{
    ReferenceType::CONSTANT, ReferenceType::SYNONYM
};

ClauseHandler::ClauseHandler(Clause *clause, PKB *pkb,
                             ClauseType validClauseType)
    : clause(clause), pkb(pkb), validClauseType(validClauseType) {}

Result ClauseHandler::eval(int ref1Index, int ref2Index) {
    Result result;
    Reference *ref1 = clause->getFirstReference();
    Reference *ref2 = clause->getSecondReference();
    ReferenceType refType1 = ref1->getRefType();
    ReferenceType refType2 = ref2->getRefType();
    string val1 = ref1->getValue();
    string val2 = ref2->getValue();

    validate();

    // same synonym
    if (refType1 == ReferenceType::SYNONYM && refType2 == ReferenceType::SYNONYM && val1 == val2) {
        set<string> res1s = getAll(*ref1);
        for (auto res1 : res1s) {
            if (isR1ClauseR2(res1, res1)) {
                result.setValid(true);
                return result;
            }
        }
        result.setValid(false);
        return result;
    }

    // WILDCARD WILDCARD
    if (refType1 == ReferenceType::WILDCARD &&
        refType2 == ReferenceType::WILDCARD) {
        set<string> res1s = getAll(*ref1);
        for (auto res1 : res1s) {
            if (getR2ClausedR1(res1).size() > 0) {
                result.setValid(true);
                return result;
            }
        }
        result.setValid(false);
        return result;
    }

    // CONSTANT CONSTANT
    if (refType1 == ReferenceType::CONSTANT &&
        refType2 == ReferenceType::CONSTANT) {
        result.setValid(isR1ClauseR2(val1, val2));
        return result;
    }

    // CONSTANT WILDCARD
    if (refType1 == ReferenceType::CONSTANT &&
        refType2 == ReferenceType::WILDCARD) {
        result.setValid(getR2ClausedR1(val1).size() > 0);
        return result;
    }

    // WILDCARD CONSTANT
    if (refType1 == ReferenceType::WILDCARD &&
        refType2 == ReferenceType::CONSTANT) {
        result.setValid(getR1ClauseR2(val2).size() > 0);
        return result;
    }

    // SYNONYM CONSTANT
    if (refType1 == ReferenceType::SYNONYM &&
        refType2 == ReferenceType::CONSTANT) {
        VALUE_TO_POINTERS_MAP firstStmtResults;
        set<string> res1s = getR1ClauseR2(val2);
        for (string res1 : res1s) {
            if (isType(res1, ref1->getDeType())) {
                firstStmtResults[res1] = POINTER_SET{};
            }
        }
        result.setResultList1(ref1, firstStmtResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (refType1 == ReferenceType::CONSTANT &&
        refType2 == ReferenceType::SYNONYM) {
        VALUE_TO_POINTERS_MAP secondStmtResults;
        set<string> res2s = getR2ClausedR1(val1);
        for (auto res2 : res2s) {
            if (isType(res2, ref2->getDeType())) {
                secondStmtResults[res2] = POINTER_SET{};
            }
        }
        result.setResultList2(ref2, secondStmtResults);
        return result;
    }

    // NEITHER IS CONSTANT
    // if first arg is SYNONYM
    if (refType1 != ReferenceType::WILDCARD) {
        VALUE_TO_POINTERS_MAP firstStmtResults;
        set<string> res1s = getAll(*ref1);

        for (string res1 : res1s) {
            set<string> res2s = getR2ClausedR1(res1);
            POINTER_SET related;
            bool valid = false;
            for (auto res2 : res2s) {
                if (isType(res2, ref2->getDeType())) {
                    valid = true;
                    if (refType2 == ReferenceType::SYNONYM) {
                        related.insert({ref2Index, res2});
                    }
                }
            }
            if (valid) {
                firstStmtResults[res1] = related;
            }
        }

        result.setResultList1(ref1, firstStmtResults);
    }

    // if second arg is SYNONYM
    if (refType2 != ReferenceType::WILDCARD) {
        VALUE_TO_POINTERS_MAP secondStmtResults;
        set<string> res2s = getAll(*ref2);

        for (string res2 : res2s) {
            set<string> res1s = getR1ClauseR2(res2);
            POINTER_SET related;
            bool valid = false;
            for (string res1 : res1s) {
                if (isType(res1, ref1->getDeType())) {
                    valid = true;
                    if (refType1 == ReferenceType::SYNONYM) {
                        related.insert({ref1Index, res1});
                    }
                }
            }
            if (valid) {
                secondStmtResults[res2] = related;
            }
        }

        result.setResultList2(ref2, secondStmtResults);
    }

    return result;
}

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

set<string> ClauseHandler::getAll(Reference ref) {
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
    if (type == DesignEntityType::PROCEDURE ||
        type == DesignEntityType::VARIABLE ||
        type == DesignEntityType::CONSTANT ||
        type == DesignEntityType::STMT) {
        return true;
    }

    StatementType stmtType = pkb->getStmtType(stoi(val));
    return DesignEntityTypeHelper::isDesTypeStmtType(type, stmtType);
}