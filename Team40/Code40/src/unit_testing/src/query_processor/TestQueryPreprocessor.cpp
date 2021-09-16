#include "catch.hpp"

#include <string>

#include "query_processor/QueryPreprocessor.h"
#include "query_processor/model/Query.h"

struct TestQPreprocessor {
    static const string INPUT_1;
    static const string INPUT_2;
    static const string INPUT_3;
    static const string INPUT_4;
    static const string PATTERN_1;
    static const string PATTERN_2;
};

const string TestQPreprocessor::INPUT_1 =
    "stmt s;\nSelect s such that Follows(s, 4)";
const string TestQPreprocessor::INPUT_2 = "stmt s;\nSelect s";
const string TestQPreprocessor::INPUT_3 = "stmt s;\nSelect p";
const string TestQPreprocessor::INPUT_4 =
    "stmt s; Select s such that Follows ( 4      ,   s)";
const string TestQPreprocessor::PATTERN_1 =
    "assign a; Select a pattern a (_, _)";
const string TestQPreprocessor::PATTERN_2 =
    "assign a; Select a pattern a(\"x\", \"y\")";

TEST_CASE("QueryPreprocessor") {
    QueryPreprocessor qp;

    SECTION("test query created correctly") {
        Query expected;
        Query actual;
        Reference ret(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
        Reference ref(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");

        SECTION("test 1") {
            Clause *cls = new Clause(ClauseType::FOLLOWS, ret, ref);
            expected.setReturnReference(&ret);
            expected.addClause(cls);

            qp.preprocessQuery(TestQPreprocessor::INPUT_1, actual);

            REQUIRE((actual.getClauses()[0])->equals(*cls));
            REQUIRE(
                (actual.getClauses()[0])->equals(*expected.getClauses()[0]));
        }

        SECTION("test 2") {
            expected.setReturnReference(&ret);
            qp.preprocessQuery(TestQPreprocessor::INPUT_2, actual);

            REQUIRE(actual.getClauses().size() == 0);
            REQUIRE((actual.getReturnReference()->equals(
                *expected.getReturnReference())));
        }

        SECTION("test 4") {
            Clause *cls = new Clause(ClauseType::FOLLOWS, ref, ret);
            expected.setReturnReference(&ret);
            expected.addClause(cls);

            qp.preprocessQuery(TestQPreprocessor::INPUT_4, actual);

            REQUIRE(cls->equals(*actual.getClauses()[0]));
            REQUIRE(expected.getClauses()[0]->equals(*actual.getClauses()[0]));
        }
    }

    SECTION("test query not created due to missing return entity") {
        Query expected;
        Query actual;
        Reference decl(DesignEntityType::STMT, ReferenceType::CONSTANT, "s");
        Reference ret(DesignEntityType::STMT, ReferenceType::SYNONYM, "p");
        expected.setReturnReference(&ret);

        REQUIRE_THROWS(qp.preprocessQuery(TestQPreprocessor::INPUT_3, actual));
    }

    SECTION("test pattern query created correctly") {
        Query expected;
        Query actual;
        Reference assign(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
        Reference var(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "x");
        Reference wildcard(DesignEntityType::VARIABLE, ReferenceType::WILDCARD,
                           "_");

        SECTION("test 1") {
            PatternClause *cls = new PatternClause(assign, wildcard, "_");
            expected.setReturnReference(&assign);
            expected.addPattern(cls);

            qp.preprocessQuery(TestQPreprocessor::PATTERN_1, actual);

            REQUIRE(actual.getPatterns()[0]->equals(*cls));
            REQUIRE(actual.getClauses().size() == 0);
        }

        SECTION("test 2") {
            PatternClause *cls = new PatternClause(assign, var, "y");
            expected.setReturnReference(&assign);
            expected.addPattern(cls);

            qp.preprocessQuery(TestQPreprocessor::PATTERN_2, actual);

            REQUIRE(actual.getPatterns()[0]->equals(*cls));
            REQUIRE(actual.getClauses().size() == 0);
        }
    }
}
