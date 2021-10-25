#include "query_processor/relationship_handler/PatternHandler.h"

PatternHandler::PatternHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::PATTERN) {
    // not used, has it own validate function
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

void PatternHandler::getFunctions(GetPatternStmtsFunc &getPatternStmts,
                                  HasPatternFunc &hasPattern) {
    switch (clause->getFirstReference()->getDeType()) {
    case DesignEntityType::ASSIGN:
        if (clause->isExactMatch()) {
            getPatternStmts = &PatternHandler::getExactAssignPatternStmts;
            hasPattern = &PatternHandler::exactAssignPattern;
        } else {
            getPatternStmts = &PatternHandler::getPartialAssignPatternStmts;
            hasPattern = &PatternHandler::partialAssignPattern;
        }
        break;
    case DesignEntityType::IF:
        getPatternStmts = &PatternHandler::getIfPatternStmts;
        hasPattern = &PatternHandler::ifPattern;
        break;
    case DesignEntityType::WHILE:
        getPatternStmts = &PatternHandler::getWhilePatternStmts;
        hasPattern = &PatternHandler::whilePattern;
        break;
    default:
        throw ClauseHandlerError(
            "PatternHandler: unknown pattern clause type!");
    }
}

set<string> PatternHandler::getR1ClauseR2(string r2) {
    set<string> res;
    GetPatternStmtsFunc getPatternStmts;
    HasPatternFunc hasPattern;
    getFunctions(getPatternStmts, hasPattern);
    for (int i : (this->*getPatternStmts)(r2, clause->getPattern())) {
        res.insert(to_string(i));
    }
    return res;
}

set<string> PatternHandler::getR2ClausedR1(string r1) {
    set<string> result;
    set<string> vars = getAll(pkb, *clause->getSecondReference());
    for (string var : vars) {
        if (isR1ClauseR2(r1, var)) {
            result.insert(var);
        }
    }
    return result;
}

bool PatternHandler::isR1ClauseR2(string r1, string r2) {
    GetPatternStmtsFunc getPatternStmts;
    HasPatternFunc hasPattern;
    getFunctions(getPatternStmts, hasPattern);
    return (this->*hasPattern)(stoi(r1), r2, clause->getPattern());
}

void PatternHandler::validate() {
    Reference *stmt = clause->getFirstReference();
    Reference *var = clause->getSecondReference();
    set<DesignEntityType> validDesTypes = {DesignEntityType::ASSIGN,
                                           DesignEntityType::WHILE,
                                           DesignEntityType::IF};
    if (validDesTypes.count(stmt->getDeType()) == 0 ||
        stmt->getRefType() != ReferenceType::SYNONYM) {
        throw ClauseHandlerError(
            "AssignPatternHandler: statement must be a synonym assigment, if, or while");
    }

    if (var->getDeType() != DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "AssignPatternHandler: second argument must be variable");
    }
}

set<int> PatternHandler::getPartialAssignPatternStmts(string var, vector<string> pattern) {
    return pkb->getPartialAssignPatternStmts(var, pattern);
}

set<int> PatternHandler::getExactAssignPatternStmts(string var, vector<string> pattern) {
    return pkb->getExactAssignPatternStmts(var, pattern);
}

set<int> PatternHandler::getIfPatternStmts(string var, vector<string> pattern) {
    return pkb->getIfPatternStmts(var);
}

set<int> PatternHandler::getWhilePatternStmts(string var, vector<string> pattern) {
    return pkb->getWhilePatternStmts(var);
}

bool PatternHandler::partialAssignPattern(int stmt, string var, vector<string> pattern) {
    return pkb->partialAssignPattern(stmt, var, pattern);
}

bool PatternHandler::exactAssignPattern(int stmt, string var, vector<string> pattern) {
    return pkb->exactAssignPattern(stmt, var, pattern);
}

bool PatternHandler::ifPattern(int stmt, string var, vector<string> pattern) {
    return pkb->ifPattern(stmt, var);
}

bool PatternHandler::whilePattern(int stmt, string var, vector<string> pattern) {
    return pkb->whilePattern(stmt, var);
}
