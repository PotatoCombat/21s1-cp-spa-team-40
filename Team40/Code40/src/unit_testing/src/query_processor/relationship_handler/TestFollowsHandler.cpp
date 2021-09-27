#include "../test_util/PKBStub.h"
#include "../test_util/PKBStub2.h"
#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/relationship_handler/FollowsHandler.h"

#include "catch.hpp"

using namespace std;

struct TestFollowsHandler {
    static PKBStub pkbStub;
    static PKBStub2 pkbStubNoFollows;
};

PKBStub TestFollowsHandler::pkbStub = PKBStub();
PKBStub2 TestFollowsHandler::pkbStubNoFollows = PKBStub2();

TEST_CASE("FollowsHandler: eval - WILDCARD WILDCARD - source has follows") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, -1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - WILDCARD WILDCARD - source does not have follows") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause,
                           &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval(-1, -1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT CONSTANT - PKB does not return -1") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, -1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT CONSTANT - PKB returns -1") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "3");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, -1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - CONSTANT WILDCARD - has stmt following CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, -1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT WILDCARD - no stmt following "
          "CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, -1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - WILDCARD CONSTANT - has stmt preceding CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, -1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - WILDCARD CONSTANT - no stmt preceding "
          "CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, -1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - SYNONYM CONSTANT - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(0, -1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList1{
        ValueToPointersMap("1", POINTER_SET{})};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - SYNONYM CONSTANT - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(0, -1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - CONSTANT SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "6");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(0, -1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList2{
        ValueToPointersMap("9", POINTER_SET{})};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - CONSTANT SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, 1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - SYNONYM WILDCARD - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(0, -1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList1{
        ValueToPointersMap("1", POINTER_SET{}),
        ValueToPointersMap("10", POINTER_SET{}),
        ValueToPointersMap("2", POINTER_SET{}),
        ValueToPointersMap("3", POINTER_SET{}),
        ValueToPointersMap("4", POINTER_SET{}),
        ValueToPointersMap("5", POINTER_SET{}),
        ValueToPointersMap("6", POINTER_SET{}),
        ValueToPointersMap("9", POINTER_SET{})};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - SYNONYM WILDCARD - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause,
                           &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval(1, -1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - WILDCARD SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, 0);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList2{
        ValueToPointersMap("10", POINTER_SET{}),
        ValueToPointersMap("11", POINTER_SET{}),
        ValueToPointersMap("12", POINTER_SET{}),
        ValueToPointersMap("2", POINTER_SET{}),
        ValueToPointersMap("3", POINTER_SET{}),
        ValueToPointersMap("4", POINTER_SET{}),
        ValueToPointersMap("6", POINTER_SET{}),
        ValueToPointersMap("9", POINTER_SET{})};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - WILDCARD SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "S");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause,
                           &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval(-1, 0);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - SYNONYM SYNONYM - returns non-empty "
          "resultList1, non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(0, 1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList1{
        ValueToPointersMap("1", POINTER_SET{make_pair(1, "2")}),
        ValueToPointersMap("10", POINTER_SET{make_pair(1, "11")}),
        ValueToPointersMap("2", POINTER_SET{make_pair(1, "3")}),
        ValueToPointersMap("3", POINTER_SET{make_pair(1, "4")}),
        ValueToPointersMap("4", POINTER_SET{make_pair(1, "12")}),
        ValueToPointersMap("5", POINTER_SET{make_pair(1, "6")}),
        ValueToPointersMap("6", POINTER_SET{make_pair(1, "9")}),
        ValueToPointersMap("9", POINTER_SET{make_pair(1, "10")})};
    vector<ValueToPointersMap> expectedList2{
        ValueToPointersMap("10", POINTER_SET{make_pair(0, "9")}),
        ValueToPointersMap("11", POINTER_SET{make_pair(0, "10")}),
        ValueToPointersMap("12", POINTER_SET{make_pair(0, "4")}),
        ValueToPointersMap("2", POINTER_SET{make_pair(0, "1")}),
        ValueToPointersMap("3", POINTER_SET{make_pair(0, "2")}),
        ValueToPointersMap("4", POINTER_SET{make_pair(0, "3")}),       
        ValueToPointersMap("6", POINTER_SET{make_pair(0, "5")}),
        ValueToPointersMap("9", POINTER_SET{make_pair(0, "6")})};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - WILDCARD SYNONYM - returns empty "
          "resultList1, empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause,
                           &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval(0, 1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList1{};
    vector<ValueToPointersMap> expectedList2{};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE(
    "FollowsHandler: eval - SYNONYM SYNONYM - filter result by stmt type") {
    Reference stmt1(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "s2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(0, 1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList1{
        ValueToPointersMap("1", POINTER_SET{make_pair(1, "2")}),
        ValueToPointersMap("2", POINTER_SET{make_pair(1, "3")})};
    vector<ValueToPointersMap> expectedList2{
        ValueToPointersMap("2", POINTER_SET{make_pair(0, "1")}),
        ValueToPointersMap("3", POINTER_SET{make_pair(0, "2")})};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - SYNONYM CONST - filter result by stmt type") {
    Reference stmt1(DesignEntityType::WHILE, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(0, -1);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONST SYNONYM - filter result by stmt type") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "9");
    Reference stmt2(DesignEntityType::CALL, ReferenceType::SYNONYM, "s");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval(-1, 0);

    Result expectedResult;
    expectedResult.setValid(true);
    vector<ValueToPointersMap> expectedList2{
        ValueToPointersMap("10", POINTER_SET{})};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}
