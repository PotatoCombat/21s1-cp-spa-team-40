#include "query_processor/relationship_handler/ModifiesProcHandler.h"

Result ModifiesProcHandler::eval(int ref1Index, int ref2Index) {
    Result result;
    // Reference *firstReference = clause->getFirstReference();
    // Reference *secondReference = clause->getSecondReference();
    // string firstValue = firstReference->getValue();
    // string secondValue = secondReference->getValue();

    //// assertions
    // validate();

    ///// CONSTANT CONSTANT
    // if (firstReference->getRefType() == ReferenceType::CONSTANT &&
    //    secondReference->getRefType() == ReferenceType::CONSTANT) {
    //    result.setValid(pkb->procModifies(firstValue, secondValue));
    //    return result;
    //}

    //// CONSTANT WILDCARD
    // if (firstReference->getRefType() == ReferenceType::CONSTANT &&
    //    secondReference->getRefType() == ReferenceType::WILDCARD) {
    //    result.setValid(pkb->getVarsModifiedByProc(firstValue).size() > 0);
    //    return result;
    //}

    //// SYNONYM CONSTANT
    // if (firstReference->getRefType() == ReferenceType::SYNONYM &&
    //    secondReference->getRefType() == ReferenceType::CONSTANT) {
    //    vector<ValueToPointersMap> procResults;
    //    set<string> procs = pkb->getProcsModifyingVar(secondValue);
    //    for (auto proc : procs) {
    //        ValueToPointersMap map(proc, POINTER_SET{});
    //        procResults.push_back(map);
    //    }
    //    result.setResultList1(firstReference, procResults);
    //    return result;
    //}

    //// CONSTANT SYNONYM
    // if (firstReference->getRefType() == ReferenceType::CONSTANT &&
    //    secondReference->getRefType() == ReferenceType::SYNONYM) {
    //    vector<ValueToPointersMap> varResults;
    //    set<string> vars = pkb->getVarsModifiedByProc(firstValue);
    //    for (auto var : vars) {
    //        ValueToPointersMap map(var, POINTER_SET{});
    //        varResults.push_back(map);
    //    }
    //    result.setResultList2(secondReference, varResults);
    //    return result;
    //}

    //// NEITHER IS CONSTANT, FIRST ARGUMENT NOT WILDCARD
    //// first arg must be SYNONYM
    // vector<ValueToPointersMap> procResults;
    // vector<string> procs = pkb->getAllProcs().asVector();

    // for (string proc : procs) {
    //    set<string> vars = pkb->getVarsModifiedByProc(proc);
    //    POINTER_SET related;
    //    bool valid = false;
    //    for (auto var : vars) {
    //        valid = true;
    //        if (secondReference->getRefType() == ReferenceType::SYNONYM) {
    //            related.insert(make_pair(ref2Index, var));
    //        }
    //    }
    //    if (valid) {
    //        ValueToPointersMap map(proc, related);
    //        procResults.push_back(map);
    //    }
    //}

    // result.setResultList1(firstReference, procResults);

    //// if second arg is SYNONYM
    // if (secondReference->getRefType() != ReferenceType::WILDCARD) {
    //    vector<ValueToPointersMap> varResults;
    //    vector<string> vars = pkb->getAllVars().asVector();
    //    for (string var : vars) {
    //        set<string> procs = pkb->getProcsModifyingVar(var);
    //        POINTER_SET related;
    //        bool valid = false;
    //        for (auto proc : procs) {
    //            valid = true;
    //            if (firstReference->getRefType() == ReferenceType::SYNONYM) {
    //                related.insert(make_pair(ref1Index, proc));
    //            }
    //        }
    //        if (valid) {
    //            ValueToPointersMap map(var, related);
    //            varResults.push_back(map);
    //        }
    //    }

    //    result.setResultList2(secondReference, varResults);
    //}

    return result;
}

void ModifiesProcHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() != DesignEntityType::PROCEDURE) {
        throw ClauseHandlerError(
            "ModifiesProcHandler: first argument must be procedure type");
    }

    if (secondReference->getDeType() != DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "ModifiesProcHandler: second argument must be variable type");
    }

    if (clause->getType() != ClauseType::MODIFIES_P) {
        throw ClauseHandlerError(
            "ModifiesProcHandler: relation type must be MODIFIES_P");
    }

    if (firstReference->getRefType() == ReferenceType::WILDCARD) {
        throw ClauseHandlerError(
            "ModifiesProcHandler: first argument cannot be wildcard");
    }
}