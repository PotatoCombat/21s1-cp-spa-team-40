#include "../test_util/PKBStub.h"
#include "../test_util/PKBStub2.h"
#include "query_processor/Result.h"
#include "query_processor/ResultCache.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/relationship_handler/FollowsStarHandler.h"

#include "catch.hpp"

using namespace std;

struct TestFollowsStarHandler {
    static PKBStub pkbStub;
    static PKBStub2 pkbStubNoFollows;
    static ResultCache cache;
};

PKBStub TestFollowsStarHandler::pkbStub = PKBStub();
PKBStub2 TestFollowsStarHandler::pkbStubNoFollows = PKBStub2();
ResultCache TestFollowsStarHandler::cache = ResultCache();

TEST_CASE("FollowsStarHandler: eval - WILDCARD WILDCARD - source has follows") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - WILDCARD WILDCARD - source does not have "
          "follows") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStubNoFollows,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - CONSTANT CONSTANT - relation holds") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "12");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE(
    "FollowsStarHandler: eval - CONSTANT CONSTANT - relation does not hold") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "5");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - CONSTANT WILDCARD - has stmt following "
          "CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - CONSTANT WILDCARD - no stmt following "
          "CONSTANT/CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - WILDCARD CONSTANT - has stmt preceding "
          "CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - WILDCARD CONSTANT - no stmt preceding "
          "CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - SYNONYM CONSTANT - returns non-empty "
          "resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "12");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList1{{"1", VALUE_SET{}},
                                      {"2", VALUE_SET{}},
                                      {"3", VALUE_SET{}},
                                      {"4", VALUE_SET{}}};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE(
    "FollowsStarHandler: eval - SYNONYM CONSTANT - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - CONSTANT SYNONYM - returns non-empty "
          "resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "6");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList2{
        {"10", VALUE_SET{}}, {"11", VALUE_SET{}}, {"9", VALUE_SET{}}};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE(
    "FollowsStarHandler: eval - CONSTANT SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}

TEST_CASE("FollowsStarHandler: eval - SYNONYM SYNONYM - returns non-empty "
          "resultList1, non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause followsStarClause(ClauseType::FOLLOWS_T, stmt1, stmt2);
    FollowsStarHandler handler(&followsStarClause,
                               &TestFollowsStarHandler::pkbStub,
                               &TestFollowsStarHandler::cache);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    map<VALUE, VALUE_SET> expectedList1{{"1", VALUE_SET{"2", "3", "4", "12"}},
                                      {"10", VALUE_SET{"11"}},
                                      {"2", VALUE_SET{"3", "4", "12"}},
                                      {"3", VALUE_SET{"4", "12"}},
                                      {"4", VALUE_SET{"12"}},
                                      {"5", VALUE_SET{"6", "9", "10", "11"}},
                                      {"6", VALUE_SET{"9", "10", "11"}},
                                      {"9", VALUE_SET{"10", "11"}}};
    
    map<VALUE, VALUE_SET> expectedList2{{"10", VALUE_SET{"5", "6", "9"}},
                                      {"11", VALUE_SET{"5", "6", "9", "10"}},
                                      {"12", VALUE_SET{"1", "2", "3", "4"}},
                                      {"2", VALUE_SET{"1"}},
                                      {"3", VALUE_SET{"1", "2"}},
                                      {"4", VALUE_SET{"1", "2", "3"}},
                                      {"6", VALUE_SET{"5"}},
                                      {"9", VALUE_SET{"5", "6"}}};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));

    TestFollowsStarHandler::cache.clear();
}
