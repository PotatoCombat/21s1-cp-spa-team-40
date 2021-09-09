#include "UsesProcHandler.h"

Result UsesProcHandler::eval() {
    Result result;
    Reference *firstReference = relation->getFirstReference();
    Reference *secondReference = relation->getSecondReference();
    string firstValue = firstReference->getValue();
    string secondValue = secondReference->getValue();

    // WILDCARD WILDCARD
    if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        vector<string> allProcs = pkb->getAllProcs().asVector();
        for (auto proc : allProcs) {
            if (pkb->getVarsUsedByProc(proc).size() > 0) {
                result.setValid(true);
            }
        }
        result.setValid(false);
        return result;
    }

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

    // WILDCARD CONSTANT
    if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        result.setValid(pkb->getProcsUsingVar(secondValue).size() > 0);
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

    // NEITHER IS CONSTANT
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

    if (firstReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList1(firstReference, procResults);
    }

    if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(secondReference, varResults);
    }

    return result;
}
