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

TEST_CASE("QueryEvaluator: 2 clauses share 2 synonyms") {
    Query query;
    Reference ifs(DesignEntityType::IF, ReferenceType::SYNONYM, "ifs");
    Reference v(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "v");
    Clause uses(ClauseType::USES_S, ifs, v);
    Clause modifies(ClauseType::MODIFIES_S, ifs, v);

    query.addReturnReference(&ifs);
    query.addReturnReference(&v);
    query.addClause(&uses);
    query.addClause(&modifies);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"6 z"});
}

TEST_CASE("QueryEvaluator: return TRUE") {
    Query query;
    Reference a(DesignEntityType::STMT, ReferenceType::SYNONYM, "a");
    Reference b(DesignEntityType::STMT, ReferenceType::SYNONYM, "b");
    Clause parentT(ClauseType::PARENT_T, a, b);

    query.addClause(&parentT);
    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"TRUE"});
}

TEST_CASE("QueryEvaluator: return FALSE") {
    Query query;
    Reference a(DesignEntityType::IF, ReferenceType::SYNONYM, "a");
    Reference b(DesignEntityType::CALL, ReferenceType::SYNONYM, "b");
    Clause follows(ClauseType::FOLLOWS, a, b);

    query.addClause(&follows);
    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"FALSE"});
}

TEST_CASE("QueryEvaluator: same reference, different attributes") {
    Query query;
    Reference c1(DesignEntityType::CALL, ReferenceType::SYNONYM, "c",
                 ReferenceAttribute::DEFAULT);
    Reference c2(DesignEntityType::CALL, ReferenceType::SYNONYM, "c",
                 ReferenceAttribute::NAME);
    query.addReturnReference(&c1);
    query.addReturnReference(&c2);
    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"10 q", "12 p"});
}

TEST_CASE("QueryEvaluator: 2 synonyms exists but not linked before") {
    Query query;
    Reference a(DesignEntityType::STMT, ReferenceType::SYNONYM, "a",
                ReferenceAttribute::INTEGER);
    Reference b(DesignEntityType::STMT, ReferenceType::SYNONYM, "b",
                ReferenceAttribute::INTEGER);
    Reference one(DesignEntityType::STMT, ReferenceType::CONSTANT, "1",
                  ReferenceAttribute::INTEGER);
    Reference two(DesignEntityType::STMT, ReferenceType::CONSTANT, "2",
                  ReferenceAttribute::INTEGER);
    Clause follows1(ClauseType::FOLLOWS, a, two);
    Clause follows2(ClauseType::FOLLOWS, one, b);
    Clause follows3(ClauseType::FOLLOWS, a, b);

    query.addReturnReference(&a);
    query.addReturnReference(&b);

    query.addClause(&follows1);
    query.addClause(&follows2);
    query.addClause(&follows3);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    REQUIRE(actual == vector<string>{"1 2"});
}

TEST_CASE("QueryEvaluator: cycle relationship") {
    Query query;
    Reference a(DesignEntityType::STMT, ReferenceType::SYNONYM, "a",
                ReferenceAttribute::INTEGER);
    Reference b(DesignEntityType::STMT, ReferenceType::SYNONYM, "b",
                ReferenceAttribute::INTEGER);
    Reference c(DesignEntityType::STMT, ReferenceType::SYNONYM, "c",
                ReferenceAttribute::INTEGER);
    Clause follows1(ClauseType::FOLLOWS, a, b);
    Clause follows2(ClauseType::FOLLOWS, b, c);
    Clause follows3(ClauseType::FOLLOWS_T, a, c);

    query.addReturnReference(&a);
    query.addReturnReference(&b);
    query.addReturnReference(&c);

    query.addClause(&follows1);
    query.addClause(&follows2);
    query.addClause(&follows3);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    vector<string> expected{"1 2 3", "2 3 4",  "3 4 12",
                            "5 6 9", "6 9 10", "9 10 11"};
    REQUIRE(actual == expected);
}

TEST_CASE("QueryEvaluator: remove duplicate") {
    Query query;
    Reference a(DesignEntityType::STMT, ReferenceType::SYNONYM, "a",
                ReferenceAttribute::INTEGER);
    Reference b(DesignEntityType::STMT, ReferenceType::SYNONYM, "b",
                ReferenceAttribute::INTEGER);
    Reference c(DesignEntityType::STMT, ReferenceType::SYNONYM, "c",
                ReferenceAttribute::INTEGER);
    Clause follows1(ClauseType::FOLLOWS, a, b);
    Clause follows2(ClauseType::FOLLOWS_T, b, c);

    query.addReturnReference(&a);
    query.addReturnReference(&b);

    query.addClause(&follows1);
    query.addClause(&follows2);

    QueryEvaluator evaluator(&TestQueryEvaluator::pkbStub);
    vector<string> actual = evaluator.evaluateQuery(query);
    vector<string> expected{"1 2", "2 3", "3 4", "5 6", "6 9", "9 10"};
    REQUIRE(actual == expected);
}
