#include "../test_util/PKBStub.h"
#include "../test_util/PKBStub2.h"
#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/relationship_handler/UsesStmtHandler.h"
#include "query_processor/relationship_handler/ClauseHandlerError.h"

#include "catch.hpp"

using namespace std;

struct TestUsesStmtHandler {
    static PKBStub pkbStub;
    //static PKBStub2 pkbStubNoParent;
};

PKBStub TestUsesStmtHandler::pkbStub = PKBStub();
//PKBStub2 TestModifiesStmtHandler::pkbStubNoParent = PKBStub2();

TEST_CASE("UsesStmtHandler: eval - stmt is WILDCARD - throws ClauseHandlerError") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
    Clause usesStmtClause(ClauseType::USES_S, stmt1, stmt2);
    UsesStmtHandler handler(&usesStmtClause, &TestUsesStmtHandler::pkbStub);

    REQUIRE_THROWS_AS(handler.eval(), ClauseHandlerError);
}


TEST_CASE("UsesStmtHandler: eval - CONSTANT CONSTANT - uses stmt match") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "8");
    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "x");
    Clause usesStmtClause(ClauseType::USES_S, stmt1, stmt2);
    UsesStmtHandler handler(&usesStmtClause, &TestUsesStmtHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("UsesStmtHandler: eval - CONSTANT CONSTANT - uses stmt not match") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "a");
    Clause usesStmtClause(ClauseType::USES_S, stmt1, stmt2);
    UsesStmtHandler handler(&usesStmtClause, &TestUsesStmtHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("UseStmtHandler: eval - CONSTANT WILDCARD - has stmt uses CONST") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
    Clause usesStmtClause(ClauseType::USES_S, stmt1, stmt2);
    UsesStmtHandler handler(&usesStmtClause, &TestUsesStmtHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("UsesStmtHandler: eval - CONSTANT WILDCARD - no stmt uses CONST") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
    Clause usesStmtClause(ClauseType::USES_S, stmt1, stmt2);
    UsesStmtHandler handler(&usesStmtClause, &TestUsesStmtHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}


TEST_CASE("UsesStmtHandler: eval - SYNONYM CONSTANT - returns resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "x");
    Clause usesStmtClause(ClauseType::USES_S, stmt1, stmt2);
    UsesStmtHandler handler(&usesStmtClause, &TestUsesStmtHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"4", "5", "6", "7", "8", "9", "10", "12"};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("UsesStmtHandler: eval - CONSTANT SYNONYM - returns resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "s");
    Clause usesStmtClause(ClauseType::USES_S, stmt1, stmt2);
    UsesStmtHandler handler(&usesStmtClause, &TestUsesStmtHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("UsesStmtHandler: eval - SYNONYM SYNONYM - returns resultList1, resultList2") {
    Reference stmt1(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "s2");
    Clause usesStmtClause(ClauseType::USES_S, stmt1, stmt2);
    UsesStmtHandler handler(&usesStmtClause, &TestUsesStmtHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"5", "7", "8", "9", "11"};
    vector<string> expectedList2{"x", "z", "i"};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}
