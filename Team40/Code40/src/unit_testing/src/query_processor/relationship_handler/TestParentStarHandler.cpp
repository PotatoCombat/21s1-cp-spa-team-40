#include "../test_util/PKBStub.h"
#include "../test_util/PKBStub2.h"
#include "Clause.h"
#include "ParentStarHandler.h"
#include "Reference.h"
#include "query_processor/Result.h"

#include "catch.hpp"

using namespace std;

struct TestParentStarHandler {
    static PKBStub pkbStub;
    static PKBStub2 pkbStubNoParent;
};

PKBStub TestParentStarHandler::pkbStub = PKBStub();
PKBStub2 TestParentStarHandler::pkbStubNoParent = PKBStub2();

TEST_CASE("ParentStarHandler: eval - WILDCARD WILDCARD - source has parent") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - WILDCARD WILDCARD - source does not have parent") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStubNoParent);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - CONSTANT CONSTANT - parent star match") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - CONSTANT CONSTANT - parent not match") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "3");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "6");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - CONSTANT WILDCARD - has stmt child CONST") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - CONSTANT WILDCARD - no stmt child CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - WILDCARD CONSTANT - has stmt parent CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "5");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - WILDCARD CONSTANT - no stmt parent CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - SYNONYM CONSTANT - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"4", "6"};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - SYNONYM CONSTANT - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - CONSTANT SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{"5", "6", "7", "8", "9", "10", "11"};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - CONSTANT SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - SYNONYM WILDCARD - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"4", "6"};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - SYNONYM WILDCARD - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStubNoParent);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - WILDCARD SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{"5", "6", "9", "10", "11", "7", "8"};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - WILDCARD SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "S");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStubNoParent);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - SYNONYM SYNONYM - returns non-empty resultList1, non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"4", "6"};
    vector<string> expectedList2{"5", "6", "9", "10", "11", "7", "8"};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("ParentStarHandler: eval - WILDCARD SYNONYM - returns empty resultList1, empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause parentStarClause(ClauseType::PARENT_T, stmt1, stmt2);
    ParentStarHandler handler(&parentStarClause, &TestParentStarHandler::pkbStubNoParent);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    vector<string> expectedList2{};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}
