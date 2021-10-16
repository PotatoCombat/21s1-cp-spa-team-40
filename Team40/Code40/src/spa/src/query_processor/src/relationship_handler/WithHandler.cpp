#include "query_processor/relationship_handler/WithHandler.h"

WithHandler::WithHandler(Clause* clause, PKB* pkb)
    : ClauseHandler(clause, pkb, ClauseType::WITH) {
    // not used
    validDesType1 = &ClauseHandler::ALL_DES_SET;
    validDesType2 = &ClauseHandler::ALL_DES_SET;
    validRefType1 = &ClauseHandler::NO_WILDCARD_REF;
    validRefType2 = &ClauseHandler::NO_WILDCARD_REF;
}

void WithHandler::validate() {
    Reference *ref1 = clause->getFirstReference();
    Reference *ref2 = clause->getSecondReference();

    ReferenceType refType1 = ref1->getRefType();
    ReferenceType refType2 = ref2->getRefType();

    DesignEntityType desType1 = ref1->getDeType();
    DesignEntityType desType2 = ref2->getDeType();

    ReferenceAttribute attr1 = ref1->getAttr();
    ReferenceAttribute attr2 = ref2->getAttr();

    // cannot be wildcard
    if (refType1 == ReferenceType::WILDCARD ||
        refType2 == ReferenceType::WILDCARD) {
        throw ClauseHandlerError("invalid reference type");
    }

    // synonym must be of type ‘prog_line’
    if (refType1 == ReferenceType::SYNONYM &&
        desType1 != DesignEntityType::STMT && 
        attr1 == ReferenceAttribute::DEFAULT) {
        throw ClauseHandlerError("with clause can only compare synonyms if they are of type stmt");
    }
    if (refType2 == ReferenceType::SYNONYM &&
        desType2 != DesignEntityType::STMT &&
        attr2 == ReferenceAttribute::DEFAULT) {
        throw ClauseHandlerError("with clause can only compare synonyms if they are of type stmt");
    }

    // the two refs must be of the same type (i.e., both strings or both
    // integers)
    bool isFirstRefName = attr1 == ReferenceAttribute::NAME;
    bool isSecondRefName = attr2 == ReferenceAttribute::NAME;
    if (isFirstRefName != isSecondRefName) {
        throw ClauseHandlerError("LHS and RHS of with clause not same type");
    }
}

set<string> WithHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (string r1 : getAll(pkb, *(clause->getFirstReference()))) {
        if (isR1ClauseR2(r1, r2)) {
            res.insert(r1);
        }
    }
    return res;
}

set<string> WithHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (string r2 : getAll(pkb, *(clause->getSecondReference()))) {
        if (isR1ClauseR2(r1, r2)) {
            res.insert(r2);
        }
    }
    return res;
}

bool WithHandler::isR1ClauseR2(string r1, string r2) {
    Reference *ref1 = clause->getFirstReference();
    Reference *ref2 = clause->getSecondReference();

    DesignEntityType desType1 = ref1->getDeType();
    DesignEntityType desType2 = ref2->getDeType();

    ReferenceAttribute attr1 = ref1->getAttr();
    ReferenceAttribute attr2 = ref2->getAttr();

    // convert r1
    string convertedR1 = r1;
    // call.procName
    if (desType1 == DesignEntityType::CALL &&
        attr1 == ReferenceAttribute::NAME) {
        convertedR1 = pkb->getCallProcedure(stoi(r1));
    }
    
    // print.varName
    if (desType1 == DesignEntityType::PRINT &&
        attr1 == ReferenceAttribute::NAME) {
        convertedR1 = pkb->getPrintVariable(stoi(r1));
    }

    // read.varName
    if (desType1 == DesignEntityType::READ &&
        attr1 == ReferenceAttribute::NAME) {
        convertedR1 = pkb->getReadVariable(stoi(r1));
    }

    // convert r2
    string convertedR2 = r2;
    // call.procName
    if (desType2 == DesignEntityType::CALL &&
        attr2 == ReferenceAttribute::NAME) {
        convertedR2 = pkb->getCallProcedure(stoi(r2));
    }

    // print.varName
    if (desType2 == DesignEntityType::PRINT &&
        attr2 == ReferenceAttribute::NAME) {
        convertedR2 = pkb->getPrintVariable(stoi(r2));
    }

    // read.varName
    if (desType2 == DesignEntityType::READ &&
        attr2 == ReferenceAttribute::NAME) {
        convertedR2 = pkb->getReadVariable(stoi(r2));
    }

    return convertedR1 == convertedR2;
}
