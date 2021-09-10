#include "../test_util/PKBStub.h"
#include "../test_util/PKBStub2.h"
#include "ParentHandler.h"
#include "Reference.h"
#include "Clause.h"
#include "Result.h"

#include "catch.hpp"

using namespace std;

struct TestParentHandler {
    static PKBStub pkbStub;
    static PKBStub2 pkbStubNoParent;
};

PKBStub TestParentHandler::pkbStub = PKBStub();
PKBStub2 TestParentHandler::pkbStubNoParent = PKBStub2();

TEST_CASE("ParentHandler: eval - WILDCARD WILDCARD - source has parent") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - WILDCARD WILDCARD - source does not have parent") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStubNoParent);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - CONSTANT CONSTANT - parent match") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "5");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - CONSTANT CONSTANT - parent not match") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "3");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "6");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - CONSTANT WILDCARD - has stmt child CONST") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - CONSTANT WILDCARD - no stmt child CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - WILDCARD CONSTANT - has stmt parent CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "5");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - WILDCARD CONSTANT - no stmt parent CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - SYNONYM CONSTANT - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "5");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"4"};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - SYNONYM CONSTANT - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - CONSTANT SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "6");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{"7", "8"};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - CONSTANT SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - SYNONYM WILDCARD - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"4", "6"};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - SYNONYM WILDCARD - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStubNoParent);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - WILDCARD SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{"5", "6", "9", "10", "11", "7", "8"};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - WILDCARD SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "S");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStubNoParent);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - SYNONYM SYNONYM - returns non-empty resultList1, non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"4", "6"};
    vector<string> expectedList2{"5", "6", "9", "10", "11", "7", "8"};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentHandler: eval - WILDCARD SYNONYM - returns empty resultList1, empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause parentClause(ClauseType::PARENT, stmt1, stmt2);
    ParentHandler handler(&parentClause, &TestParentHandler::pkbStubNoParent);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    vector<string> expectedList2{};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}
