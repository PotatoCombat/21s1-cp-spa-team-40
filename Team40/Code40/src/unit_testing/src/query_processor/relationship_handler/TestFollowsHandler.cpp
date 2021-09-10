#include "FollowsHandler.h"
#include "../test_util/PKBStub.h"
#include "../test_util/PKBStub2.h"
#include "Result.h"
#include "Reference.h"
#include "Relation.h"

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
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - WILDCARD WILDCARD - source does not have follows") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT CONSTANT - PKB does not return -1") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT CONSTANT - PKB returns -1") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "3");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT WILDCARD - has stmt following CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT WILDCARD - no stmt following CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - WILDCARD CONSTANT - has stmt preceding CONSTANT") {
    Result expectedResult;
    expectedResult.setValid(true);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - WILDCARD CONSTANT - no stmt preceding CONSTANT/ CONSTANT out of bound") {
    Result expectedResult;
    expectedResult.setValid(false);

    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - SYNONYM CONSTANT - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"1"};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - SYNONYM CONSTANT - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "6");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{"9"};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - CONSTANT SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "7");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - SYNONYM WILDCARD - returns non-empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"1", "2", "3", "4", "5", "6", "9", "10"};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - SYNONYM WILDCARD - returns empty resultList1") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    expectedResult.setResultList1(&stmt1, expectedList1);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - WILDCARD SYNONYM - returns non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{"2", "3", "4", "12", "6", "9", "10", "11"};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - WILDCARD SYNONYM - returns empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "S");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList2{};
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - SYNONYM SYNONYM - returns non-empty resultList1, non-empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStub);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{"1", "2", "3", "4", "5", "6", "9", "10"};
    vector<string> expectedList2{"2", "3", "4", "12", "6", "9", "10", "11"};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}

TEST_CASE("FollowsHandler: eval - WILDCARD SYNONYM - returns empty resultList1, empty resultList2") {
    Reference stmt1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference stmt2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Relation followsRelation(RelationType::FOLLOWS, &stmt1, &stmt2);
    FollowsHandler handler(&followsRelation, &TestFollowsHandler::pkbStubNoFollows);
    Result actualResult = handler.eval();

    Result expectedResult;
    expectedResult.setValid(true);
    vector<string> expectedList1{};
    vector<string> expectedList2{};
    expectedResult.setResultList1(&stmt1, expectedList1);
    expectedResult.setResultList2(&stmt2, expectedList2);

    REQUIRE(expectedResult.equals(actualResult));
}
