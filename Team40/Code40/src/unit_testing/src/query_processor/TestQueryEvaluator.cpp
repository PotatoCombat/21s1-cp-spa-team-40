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
    REQUIRE(actual == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8",
                                     "9", "10", "11", "12"});
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
