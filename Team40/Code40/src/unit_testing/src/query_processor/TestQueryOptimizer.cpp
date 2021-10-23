#include "query_processor/QueryOptimizer.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Query.h"
#include "query_processor/model/Reference.h"

#include "catch.hpp"

TEST_CASE("QueryOptimizer: clauses that return boolean moved to front") {
    Reference a(DesignEntityType::STMT, ReferenceType::SYNONYM, "a");
    Reference b(DesignEntityType::STMT, ReferenceType::SYNONYM, "b");

    Reference wc1(DesignEntityType::STMT, ReferenceType::WILDCARD, "wc1");
    Reference wc2(DesignEntityType::STMT, ReferenceType::WILDCARD, "wc2");

    Reference c1(DesignEntityType::STMT, ReferenceType::CONSTANT, "c1");
    Reference c2(DesignEntityType::STMT, ReferenceType::CONSTANT, "c2");

    Clause follows1(ClauseType::FOLLOWS, a, b);
    Clause follows2(ClauseType::FOLLOWS, a, wc1);
    Clause follows3(ClauseType::FOLLOWS, c1, a);

    SECTION("WILDCARD WILDCARD") {
        Query q;
        Clause cls(ClauseType::FOLLOWS, wc1, wc2);
        q.addClause(&follows1);
        q.addClause(&cls);

        Query after;
        QueryOptimizer::optimize(q, after);
        vector<Clause *> optimized = after.getClauses();
        REQUIRE(optimized.size() == 2);
        REQUIRE(optimized[0]->equals(cls));
        REQUIRE(optimized[1]->equals(follows1));
    }

    SECTION("WILDCARD CONST") {
        Query q;
        Clause cls(ClauseType::FOLLOWS, wc1, c1);
        q.addClause(&follows1);
        q.addClause(&cls);

        Query after;
        QueryOptimizer::optimize(q, after);
        vector<Clause *> optimized = after.getClauses();
        REQUIRE(optimized.size() == 2);
        REQUIRE(optimized[0]->equals(cls));
        REQUIRE(optimized[1]->equals(follows1));
    }

    SECTION("CONST WILDCARD") {
        Query q;
        Clause cls(ClauseType::FOLLOWS, c1, wc1);
        q.addClause(&follows1);
        q.addClause(&cls);

        Query after;
        QueryOptimizer::optimize(q, after);
        vector<Clause *> optimized = after.getClauses();
        REQUIRE(optimized.size() == 2);
        REQUIRE(optimized[0]->equals(cls));
        REQUIRE(optimized[1]->equals(follows1));
    }

    SECTION("CONST CONST") {
        Query q;
        Clause cls(ClauseType::FOLLOWS, c1, c2);
        q.addClause(&follows1);
        q.addClause(&cls);

        Query after;
        QueryOptimizer::optimize(q, after);
        vector<Clause *> optimized = after.getClauses();
        REQUIRE(optimized.size() == 2);
        REQUIRE(optimized[0]->equals(cls));
        REQUIRE(optimized[1]->equals(follows1));
    }
}

TEST_CASE("QueryOptimizer: sort by group size") {
    Reference a(DesignEntityType::STMT, ReferenceType::SYNONYM, "a");
    Reference b(DesignEntityType::STMT, ReferenceType::SYNONYM, "b");
    Reference c(DesignEntityType::STMT, ReferenceType::SYNONYM, "c");

    Reference wc1(DesignEntityType::STMT, ReferenceType::WILDCARD, "wc1");

    Clause group1_1(ClauseType::FOLLOWS, a, b);
    Clause group1_2(ClauseType::PARENT, a, b);
    Clause group1_3(ClauseType::PARENT_T, a, b);

    Clause group2_1(ClauseType::FOLLOWS, c, wc1);

    Query q;
    q.addClause(&group1_1);
    q.addClause(&group1_2);
    q.addClause(&group1_3);
    q.addClause(&group2_1);

    Query after;
    QueryOptimizer::optimize(q, after);
    vector<Clause *> optimized = after.getClauses();
    REQUIRE(optimized.size() == 4);
    REQUIRE(optimized[0]->equals(group2_1));
    REQUIRE(optimized[1]->equals(group1_1));
    REQUIRE(optimized[2]->equals(group1_2));
    REQUIRE(optimized[3]->equals(group1_3));
}
