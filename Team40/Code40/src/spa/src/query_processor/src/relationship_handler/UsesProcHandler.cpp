#include "UsesProcHandler.h"

Result UsesProcHandler::eval() {
    Result result;
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    string firstValue = firstReference->getValue();
    string secondValue = secondReference->getValue();

    // assertions
    validate();

    /// CONSTANT CONSTANT
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        result.setValid(pkb->procUses(firstValue, secondValue));
        return result;
    }

    // CONSTANT WILDCARD
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        result.setValid(pkb->getVarsUsedByProc(firstValue).size() > 0);
        return result;
    }

    // SYNONYM CONSTANT
    if (firstReference->getRefType() == ReferenceType::SYNONYM &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        vector<string> procResults;
        set<string> procs = pkb->getProcsUsingVar(secondValue);
        for (auto proc : procs) {
            procResults.push_back(proc);
        }
        result.setResultList1(firstReference, procResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<string> varResults;
        set<string> vars = pkb->getVarsUsedByProc(firstValue);
        for (auto var : vars) {
            varResults.push_back(var);
        }
        result.setResultList2(secondReference, varResults);
        return result;
    }

    // NEITHER IS CONSTANT, FIRST ARGUMENT NOT WILDCARD
    vector<string> procResults;
    vector<string> varResults;
    vector<string> procs = pkb->getAllProcs().asVector();
    for (auto proc : procs) {
        set<string> vars = pkb->getVarsUsedByProc(proc);
        if (vars.size() == 0) {
            continue;
        }
        procResults.push_back(proc);
        for (auto var : vars) {
            if (find(varResults.begin(), varResults.end(), var) ==
                varResults.end()) {
                varResults.push_back(var);
            }
        }
    }

    result.setResultList1(firstReference, procResults);

    if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(secondReference, varResults);
    }

    return result;
}

void UsesProcHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() != DesignEntityType::PROCEDURE) {
        throw ClauseHandlerError("UsesProcHandler: first argument must be procedure type");
    }

    if (secondReference->getDeType() != DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("UsesProcHandler: second argument must be variable type");
    }

    if (clause->getType() != ClauseType::USES_P) {
        throw ClauseHandlerError("UsesProcHandler: relation type must be USES_P");
    }

    if (firstReference->getRefType() == ReferenceType::WILDCARD) {
        throw ClauseHandlerError("UsesProcHandler: first argument cannot be wildcard");
    }

}
