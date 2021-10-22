#include "query_processor/relationship_handler/PatternHandler.h"

Result PatternHandler::eval() {
    Result result;

    validate();

    Reference *stmtRef = patternClause->getFirstReference();
    Reference *varRef = patternClause->getSecondReference();
    vector<string> pattern = patternClause->getPattern();

    set<int> (PatternHandler::*getPatternStmts)(string, vector<string>);
    bool (PatternHandler::*hasPattern)(int, string, vector<string>);
    StatementType stmtType;

    switch (stmtRef->getDeType()) {
        case DesignEntityType::ASSIGN:
            if (patternClause->isExactMatch()) {
                getPatternStmts = &PatternHandler::getExactAssignPatternStmts;
                hasPattern = &PatternHandler::exactAssignPattern;
            } else {
                getPatternStmts = &PatternHandler::getPartialAssignPatternStmts;
                hasPattern = &PatternHandler::partialAssignPattern;
            }
            stmtType = StatementType::ASSIGN;
            break;
        case DesignEntityType::IF:
            getPatternStmts = &PatternHandler::getIfPatternStmts;
            hasPattern = &PatternHandler::ifPattern;
            stmtType = StatementType::IF;
            break;
        case DesignEntityType::WHILE:
            getPatternStmts = &PatternHandler::getWhilePatternStmts;
            hasPattern = &PatternHandler::whilePattern;
            stmtType = StatementType::WHILE;
            break;
        default:
            throw ClauseHandlerError("PatternHandler: unknown pattern clause type!");
    }

    // SYNONYM CONST
    if (varRef->getRefType() == ReferenceType::CONSTANT) {
        map<VALUE, VALUE_SET> stmtResults;
        set<int> stmts =
            (this->*getPatternStmts)(varRef->getValue(), pattern);
        for (auto stmt : stmts) {
            stmtResults[to_string(stmt)] = VALUE_SET{};
        }
        result.setResultList1(stmtRef, stmtResults);
        return result;
    }

    // SYNONYM SYNONYM, SYNONYM WILDCARD
    map<VALUE, VALUE_SET> stmtResults;
    map<VALUE, VALUE_SET> varResults;
    vector<int> assigns = pkb->getAllStmts(stmtType).asVector();
    vector<string> vars = pkb->getAllVars().asVector();

    for (int assign : assigns) {
        VALUE_SET related;
        bool valid = false;
        for (string var : vars) {
            if ((this->*hasPattern)(assign, var, pattern)) {
                valid = true;
                related.insert(var);
            }
        }
        if (valid) {
            stmtResults[to_string(assign)] = related;
        }
    }
    
    for (string var : vars) {
        VALUE_SET related;
        bool valid = false;
        for (int assign : assigns) {
            if ((this->*hasPattern)(assign, var, pattern)) {
                valid = true;
                if (stmtRef->getRefType() == ReferenceType::SYNONYM) {
                    related.insert(to_string(assign));
                }
            }
        }
        if (valid) {
            varResults[var] = related;
        }
    }

    result.setResultList1(stmtRef, stmtResults);

    if (varRef->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(varRef, varResults);
    }

    return result;
}

void PatternHandler::validate() {
    Reference *stmt = patternClause->getFirstReference();
    Reference *var = patternClause->getSecondReference();
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
