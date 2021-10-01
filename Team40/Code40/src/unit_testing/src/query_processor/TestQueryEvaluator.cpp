#include "query_processor/QueryEvaluator.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Query.h"
#include "query_processor/model/Reference.h"

#include "test_util/PKBStub.h"
#include "test_util/PKBStub2.h"

#include "catch.hpp"

using namespace std;

struct TestQueryEvaluator {
    static PKBStub pkbStub;
    static PKBStub2 pkbStubNoFollows;
};

PKBStub TestQueryEvaluator::pkbStub = PKBStub();
PKBStub2 TestQueryEvaluator::pkbStubNoFollows = PKBStub2();

TEST_CASE("QueryEvaluator: no clause - returns all") {
    Query query;
    Reference c(DesignEntityType::CONSTANT, ReferenceType::SYNONYM, "c");
    query.addReturnReference(&c);
    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"1", "2", "3", "5"});
}

TEST_CASE("QueryEvaluator: one clause returns false - returns empty result") {
    Query query;
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference s1(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Reference s2(DesignEntityType::STMT, ReferenceType::CONSTANT, "12");
    Clause follows(ClauseType::FOLLOWS, s1, s2);

    query.addReturnReference(&s);
    query.addClause(&follows);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{});
}

TEST_CASE("QueryEvaluator: all clauses return true - returns all statements") {
    Query query;
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference s_const1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference s_const2(DesignEntityType::STMT, ReferenceType::CONSTANT, "2");
    Clause follows(ClauseType::FOLLOWS, s_const1, s_const2);
    Reference stmt_sym1(DesignEntityType::STMT, ReferenceType::SYNONYM, "k");
    Reference s_const3(DesignEntityType::STMT, ReferenceType::CONSTANT, "12");
    Clause follows2(ClauseType::FOLLOWS, stmt_sym1, s_const3);

    query.addReturnReference(&s);
    query.addClause(&follows);
    query.addClause(&follows2);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"1", "10", "11", "12", "2", "3", "4", "5",
                                     "6", "7", "8", "9"});
}

TEST_CASE("QueryEvaluator: one clause return empty list - returns empty list") {
    Query query;
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference stmt_sym1(DesignEntityType::STMT, ReferenceType::SYNONYM, "k");
    Reference s_const3(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause follows(ClauseType::FOLLOWS, stmt_sym1, s_const3);

    query.addReturnReference(&s);
    query.addClause(&follows);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStubNoFollows);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{});
}

TEST_CASE(
    "QueryEvaluator: no intersection between results - returns empty list") {
    Query query;
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference s1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference s2(DesignEntityType::STMT, ReferenceType::CONSTANT, "12");
    Clause follows(ClauseType::FOLLOWS, s1, s);
    Clause follows2(ClauseType::FOLLOWS, s, s2);

    query.addReturnReference(&s);
    query.addClause(&follows);
    query.addClause(&follows2);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{});
}

TEST_CASE("QueryEvaluator: 1 matching element between results - returns the "
          "intersection") {
    Query query;
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference s1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
    Reference s2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause follows(ClauseType::FOLLOWS, s1, s);
    Clause follows2(ClauseType::FOLLOWS, s, s2);

    query.addReturnReference(&s);
    query.addClause(&follows);
    query.addClause(&follows2);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"2"});
}

TEST_CASE("QueryEvaluator: multiple matching elements between results - "
          "returns the intersection") {
    Query query;
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference s1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference s2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause follows(ClauseType::FOLLOWS, s1, s);
    Clause follows2(ClauseType::FOLLOWS, s, s2);

    query.addReturnReference(&s);
    query.addClause(&follows);
    query.addClause(&follows2);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"10", "2", "3", "4", "6", "9"});
}

TEST_CASE("QueryEvaluator: intersection not return element") {
    Query query;
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference s1(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1");
    Reference c(DesignEntityType::STMT, ReferenceType::CONSTANT, "9");
    Clause follows(ClauseType::FOLLOWS, s, s1);
    Clause follows2(ClauseType::FOLLOWS, s1, c);
    
    query.addReturnReference(&s);
    query.addClause(&follows);
    query.addClause(&follows2);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"5"});
}

TEST_CASE("QueryEvaluator: return multi result 1") {
    Query query;
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference s1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference s2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause follows(ClauseType::FOLLOWS, s1, s);
    Clause follows2(ClauseType::FOLLOWS, s, s2);

    query.addReturnReference(&s);
    query.addReturnReference(&s2);
    query.addClause(&follows);
    query.addClause(&follows2);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"10 11", "2 3", "3 4", "4 12", "6 9", "9 10"});
}

TEST_CASE("QueryEvaluator: return multi result 2") {
    Query query;
    Reference c(DesignEntityType::CALL, ReferenceType::SYNONYM, "c");
    Reference s(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference s1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Reference s2(DesignEntityType::STMT, ReferenceType::SYNONYM, "s2");
    Clause follows(ClauseType::FOLLOWS, s1, s);
    Clause follows2(ClauseType::FOLLOWS, s, s2);

    query.addReturnReference(&s);
    query.addReturnReference(&c);
    query.addReturnReference(&s2);
    query.addClause(&follows);
    query.addClause(&follows2);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"10 10 11", "10 12 11", "2 10 3", "2 12 3",
                                     "3 10 4", "3 12 4", "4 10 12", "4 12 12",
                                     "6 10 9", "6 12 9", "9 10 10", "9 12 10"});
}
