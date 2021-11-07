#include "catch.hpp"

#include "../test_util/PKBStub.h"
#include "../test_util/PKBStub2.h"

#include "query_processor/ResultCache.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/model/Result.h"

#include "query_processor/relationship_handler/FollowsHandler.h"

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
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
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
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE("FollowsHandler: eval - CONSTANT CONSTANT - PKB does not return -1") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE("FollowsHandler: eval - CONSTANT CONSTANT - PKB returns -1") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "3");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE(
    "FollowsHandler: eval - CONSTANT WILDCARD - has stmt following CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE("FollowsHandler: eval - CONSTANT WILDCARD - no stmt following "
          "CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE(
    "FollowsHandler: eval - WILDCARD CONSTANT - has stmt preceding CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE("FollowsHandler: eval - WILDCARD CONSTANT - no stmt preceding "
          "CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE(
    "FollowsHandler: eval - SYNONYM CONSTANT - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList1{{"1", VALUE_SET{}}};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE(
    "FollowsHandler: eval - SYNONYM CONSTANT - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE(
    "FollowsHandler: eval - CONSTANT SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "6");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList2{{"9", VALUE_SET{}}};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE(
    "FollowsHandler: eval - CONSTANT SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE(
    "FollowsHandler: eval - SYNONYM SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "a");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "S");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause,
                           &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList1{};
    map<VALUE, VALUE_SET> expectedList2{};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}

TEST_CASE("FollowsHandler: eval - SYNONYM SYNONYM - returns non-empty "
          "resultList1, non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause followsClause(ClauseType::FOLLOWS, stmt1, stmt2);
    FollowsHandler handler(&followsClause, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList1{
        {"1", VALUE_SET{"2"}}, {"10", VALUE_SET{"11"}}, {"2", VALUE_SET{"3"}},
        {"3", VALUE_SET{"4"}}, {"4", VALUE_SET{"12"}},  {"5", VALUE_SET{"6"}},
        {"6", VALUE_SET{"9"}}, {"9", VALUE_SET{"10"}}};
    map<VALUE, VALUE_SET> expectedList2{
        {"10", VALUE_SET{"9"}}, {"11", VALUE_SET{"10"}}, {"12", VALUE_SET{"4"}},
        {"2", VALUE_SET{"1"}},  {"3", VALUE_SET{"2"}},   {"4", VALUE_SET{"3"}},
        {"6", VALUE_SET{"5"}},  {"9", VALUE_SET{"6"}}};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));

    ResultCache::getInstance().clear();
}
