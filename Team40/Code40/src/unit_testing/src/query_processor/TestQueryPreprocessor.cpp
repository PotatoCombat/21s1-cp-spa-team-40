#include "catch.hpp"

#include <string>

#include "query_processor/QueryPreprocessor.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Query.h"
#include "query_processor/parser/ParserUtil.h"

struct TestQPreprocessor {
    static const string INPUT_1;
    static const string INPUT_2;
    static const string INPUT_3;
    static const string INPUT_4;
    static const string INPUT_5;
    static const string INPUT_6;
    static const string PATTERN_1;
    static const string PATTERN_2;
    static const string SUCH_THAT_PATTERN_1;
    static const string SUCH_THAT_PATTERN_2;
    static const string WITH_1;
    static const string WITH_2;
    static void buildQuery(pair<string, string> clauseArgs, Query &q);
};

const string TestQPreprocessor::INPUT_1 =
    "stmt s;\nSelect s such that Follows(s, 4)";
const string TestQPreprocessor::INPUT_2 = "stmt s;\nSelect s";
const string TestQPreprocessor::INPUT_3 = "stmt s;\nSelect p";
const string TestQPreprocessor::INPUT_4 =
    "stmt s; Select s such that Follows ( 4      ,   s)";
const string TestQPreprocessor::INPUT_5 =
    "\rif ifs;\r\nSelect ifs such that Follows(4, ifs)\r";
const string TestQPreprocessor::INPUT_6 =
    "assign a; variable v; Select v such that Uses(a, \"  height\")";
const string TestQPreprocessor::PATTERN_1 =
    "assign a; Select a pattern a (_, _)";
const string TestQPreprocessor::PATTERN_2 =
    "assign a; Select a pattern a(\"x\", \"y\")";
const string TestQPreprocessor::SUCH_THAT_PATTERN_1 =
    "\rstmt s; procedure PROC3DURE; assign a; Select s such that "
    "Uses(PROC3DURE, \"y\") pattern a(\"x\", \" y \")\r";
const string TestQPreprocessor::SUCH_THAT_PATTERN_2 =
    "stmt s; assign a; Select s such that Follows*(s, a) pattern a(\"   d\", "
    "_\"a \"_)";
const string TestQPreprocessor::WITH_1 =
    "assign a; prog_line n;\nSelect a with a.stmt# = n and a.stmt# = 10";
const string TestQPreprocessor::WITH_2 =
    "call c; procedure p;\nSelect c with c.procName = p.procName and c.stmt# = "
    "10";

void TestQPreprocessor::buildQuery(pair<string, string> clauseArgs, Query &q) {
    Reference *WILDCARD =
        new Reference(DesignEntityType::STMT, ReferenceType::WILDCARD, "_",
                      ReferenceAttribute::INTEGER);
    Reference *ONE =
        new Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "1",
                      ReferenceAttribute::INTEGER);
    Reference *s1 =
        new Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s1",
                      ReferenceAttribute::INTEGER);
    Reference *r1 = s1;
    Reference *r2 = s1;

    q.addReturnReference(r1);

    string arg1 = clauseArgs.first;
    string arg2 = clauseArgs.second;
    if (arg1 == "_") {
        r1 = WILDCARD;
    } else if (arg1 == "1") {
        r1 = ONE;
    }

    if (arg2 == "_") {
        r2 = WILDCARD;
    } else if (arg2 == "1") {
        r2 = ONE;
    } else if (arg2 == "s2") {
        r2 = new Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, arg2,
                           ReferenceAttribute::INTEGER);
    } else if (arg2 == "a") {
        r2 = new Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM,
                           arg2, ReferenceAttribute::INTEGER);
    } else if (arg2 == "n") {
        r2 = new Reference(DesignEntityType::PROG_LINE, ReferenceType::SYNONYM,
                           arg2, ReferenceAttribute::INTEGER);
    } else if (arg2 == "v") {
        r2 = new Reference(DesignEntityType::VARIABLE, ReferenceType::SYNONYM,
                           arg2, ReferenceAttribute::NAME);
    } else if (arg2 == "p") {
        r2 = new Reference(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM,
                           arg2, ReferenceAttribute::NAME);
    } else if (arg2 == "c") {
        r2 = new Reference(DesignEntityType::CONSTANT, ReferenceType::SYNONYM,
                           arg2, ReferenceAttribute::INTEGER);
    }

    Clause *c = new Clause(ClauseType::FOLLOWS, *r1, *r2);
    q.addClause(c);
}

TEST_CASE("QueryPreprocessor: parse all valid and invalid Follows queries") {
    QueryPreprocessor qp;
    string q;
    Query expected;
    Query actual;

    SECTION("at least one non-synonym argument") {
        SECTION("PASS: first argument is wildcard") {
            q = "stmt s1; Select s1 such that Follows(_, _)";
            TestQPreprocessor::buildQuery(make_pair("_", "_"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
            expected = Query();
            actual = Query();

            q = "stmt s1; Select s1 such that Follows(_, 1)";
            TestQPreprocessor::buildQuery(make_pair("_", "1"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
            expected = Query();
            actual = Query();

            q = "stmt s1; Select s1 such that Follows(_, s1)";
            TestQPreprocessor::buildQuery(make_pair("_", "s1"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
        }

        SECTION("PASS: first argument is integer") {
            q = "stmt s1; Select s1 such that Follows(1, _)";
            TestQPreprocessor::buildQuery(make_pair("1", "_"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
            expected = Query();
            actual = Query();

            q = "stmt s1; Select s1 such that Follows(1, 1)";
            TestQPreprocessor::buildQuery(make_pair("1", "1"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
            expected = Query();
            actual = Query();

            q = "stmt s1; Select s1 such that Follows(1, s1)";
            TestQPreprocessor::buildQuery(make_pair("1", "s1"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
        }

        SECTION("FAIL: argument is quoted") {
            q = "stmt s1; Select s1 such that Follows(\"1\", 1)";
            REQUIRE_FALSE(qp.preprocessQuery(q, actual));
            actual = Query();

            q = "stmt s1; Select s1 such that Follows(1, \"1\")";
            REQUIRE_FALSE(qp.preprocessQuery(q, actual));
            actual = Query();

            q = "stmt s1; Select s1 such that Follows(\"name\", 1)";
            REQUIRE_FALSE(qp.preprocessQuery(q, actual));
            actual = Query();

            q = "stmt s1; Select s1 such that Follows(1, \"name\")";
            REQUIRE_FALSE(qp.preprocessQuery(q, actual));
        }
    }

    SECTION("two synonym arguments") {
        SECTION("PASS: argument is statement-typed") {
            q = "stmt s1, s2; Select s1 such that Follows(s1, s2)";
            TestQPreprocessor::buildQuery(make_pair("s1", "s2"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
            expected = Query();
            actual = Query();

            q = "stmt s1; assign a; Select s1 such that Follows(s1, a)";
            TestQPreprocessor::buildQuery(make_pair("s1", "a"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
            expected = Query();
            actual = Query();

            q = "stmt s1; prog_line n; Select s1 such that Follows(s1, n)";
            TestQPreprocessor::buildQuery(make_pair("s1", "n"), expected);
            REQUIRE(qp.preprocessQuery(q, actual));
            REQUIRE(actual.equals(expected));
        }

        SECTION("FAIL: argument is not statement-typed") {
            q = "stmt s1; variable v; Select s1 such that Follows(s1, v)";
            REQUIRE_FALSE(qp.preprocessQuery(q, actual));
            actual = Query();

            q = "stmt s1; procedure p; Select s1 such that Follows(s1, p)";
            REQUIRE_FALSE(qp.preprocessQuery(q, actual));
            actual = Query();

            q = "stmt s1; constant c; Select s1 such that Follows(s1, c)";
            REQUIRE_FALSE(qp.preprocessQuery(q, actual));
        }
    }
}

TEST_CASE("QueryPreprocessor: single clause") {
    QueryPreprocessor qp;

    SECTION("test query created correctly") {
        Query expected;
        Query actual;
        Reference ret(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
        Reference ref(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");

        SECTION("test 1") {
            Clause *cls = new Clause(ClauseType::FOLLOWS, ret, ref);
            expected.addReturnReference(&ret);
            expected.addClause(cls);

            qp.preprocessQuery(TestQPreprocessor::INPUT_1, actual);

            REQUIRE((actual.getClauses()[0])->equals(*cls));
            REQUIRE(
                (actual.getClauses()[0])->equals(*expected.getClauses()[0]));
        }

        SECTION("test 2") {
            expected.addReturnReference(&ret);
            qp.preprocessQuery(TestQPreprocessor::INPUT_2, actual);

            REQUIRE(actual.getClauses().size() == 0);
            REQUIRE((actual.getReturnReferences()[0])
                        ->equals(*expected.getReturnReferences()[0]));
        }

        SECTION("test 4") {
            Clause *cls = new Clause(ClauseType::FOLLOWS, ref, ret);
            expected.addReturnReference(&ret);
            expected.addClause(cls);

            qp.preprocessQuery(TestQPreprocessor::INPUT_4, actual);

            REQUIRE(cls->equals(*actual.getClauses()[0]));
            REQUIRE(expected.getClauses()[0]->equals(*actual.getClauses()[0]));
        }

        SECTION("test 5") {
            Reference ifs(DesignEntityType::IF, ReferenceType::SYNONYM, "ifs");
            Clause *cls = new Clause(ClauseType::FOLLOWS, ref, ifs);
            expected.addReturnReference(&ifs);
            expected.addClause(cls);

            qp.preprocessQuery(TestQPreprocessor::INPUT_5, actual);
            REQUIRE(cls->equals(*actual.getClauses()[0]));
            REQUIRE(expected.getClauses()[0]->equals(*actual.getClauses()[0]));
        }

        SECTION("test 6") {
            Reference a(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
            Reference v(DesignEntityType::VARIABLE, ReferenceType::SYNONYM,
                        "v");
            Reference height(DesignEntityType::VARIABLE,
                             ReferenceType::CONSTANT, "height");
            Clause *cls = new Clause(ClauseType::USES_S, a, height);
            expected.addReturnReference(&v);
            expected.addClause(cls);

            qp.preprocessQuery(TestQPreprocessor::INPUT_6, actual);
            REQUIRE(cls->equals(*actual.getClauses()[0]));
            REQUIRE(expected.getClauses()[0]->equals(*actual.getClauses()[0]));
        }
    }

    SECTION("test query not created due to missing return entity") {
        Query expected;
        Query actual;
        Reference decl(DesignEntityType::STMT, ReferenceType::CONSTANT, "s");
        Reference ret(DesignEntityType::STMT, ReferenceType::SYNONYM, "p");
        expected.addReturnReference(&ret);

        REQUIRE_FALSE(qp.preprocessQuery(TestQPreprocessor::INPUT_3, actual));
    }

    SECTION("test pattern query created correctly") {
        Query expected;
        Query actual;
        Reference assign(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
        Reference var(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "x");
        Reference wildcard(DesignEntityType::VARIABLE, ReferenceType::WILDCARD,
                           "_");

        SECTION("test 1") {
            Clause *cls = new Clause(ClauseType::PATTERN, assign, wildcard);
            expected.addReturnReference(&assign);
            expected.addClause(cls);

            qp.preprocessQuery(TestQPreprocessor::PATTERN_1, actual);

            REQUIRE(actual.getClauses()[0]->equals(*cls));
        }

        SECTION("test 2") {
            Clause *cls = new Clause(assign, var, vector<string>{"y"}, true);
            expected.addReturnReference(&assign);
            expected.addClause(cls);

            qp.preprocessQuery(TestQPreprocessor::PATTERN_2, actual);

            REQUIRE(actual.getClauses()[0]->equals(*cls));
        }
    }
}

TEST_CASE("QueryPreprocessor: two clauses") {
    QueryPreprocessor qp;
    Query expected;
    Query actual;
    Reference procedure(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM,
                        "PROC3DURE");
    Reference stmt(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference assign(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
    Reference variableD(DesignEntityType::VARIABLE, ReferenceType::CONSTANT,
                        "d");
    Reference variableX(DesignEntityType::VARIABLE, ReferenceType::CONSTANT,
                        "x");
    Reference variableY(DesignEntityType::VARIABLE, ReferenceType::CONSTANT,
                        "y");

    SECTION("test 1") {
        Clause *cls = new Clause(ClauseType::USES_P, procedure, variableY);
        Clause *pat = new Clause(assign, variableX, vector<string>{"y"}, true);
        expected.addReturnReference(&stmt);
        expected.addClause(cls);
        expected.addClause(pat);

        qp.preprocessQuery(TestQPreprocessor::SUCH_THAT_PATTERN_1, actual);

        REQUIRE((actual.getClauses()[0])->equals(*cls));
        REQUIRE((actual.getClauses()[1])->equals(*pat));
        REQUIRE((actual.getReturnReferences()[0])->equals(stmt));
        REQUIRE((actual.getReferences().size() == 3));
    }

    SECTION("test 2") {
        Clause *cls = new Clause(ClauseType::FOLLOWS_T, stmt, assign);
        Clause *pat = new Clause(assign, variableD, vector<string>{"a"}, false);
        expected.addReturnReference(&stmt);
        expected.addClause(cls);
        expected.addClause(pat);

        qp.preprocessQuery(TestQPreprocessor::SUCH_THAT_PATTERN_2, actual);

        REQUIRE((actual.getClauses()[0])->equals(*cls));
        REQUIRE((actual.getClauses()[1])->equals(*pat));
        REQUIRE((actual.getReturnReferences()[0])->equals(stmt));
        REQUIRE((actual.getReferences().size() == 2));
    }
}

TEST_CASE("QueryPreprocessor: with clauses") {
    QueryPreprocessor qp;
    Query expected;
    Query actual;
    Reference assign(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a",
                     ReferenceAttribute::INTEGER);
    Reference procedure(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM,
                        "p", ReferenceAttribute::NAME);
    Reference prog_line(DesignEntityType::PROG_LINE, ReferenceType::SYNONYM,
                        "n", ReferenceAttribute::INTEGER);
    Reference call(DesignEntityType::CALL, ReferenceType::SYNONYM, "c",
                   ReferenceAttribute::INTEGER);
    Reference call_p(DesignEntityType::CALL, ReferenceType::SYNONYM, "c",
                     ReferenceAttribute::NAME);
    Reference constant(DesignEntityType::STMT, ReferenceType::CONSTANT, "10",
                       ReferenceAttribute::INTEGER);

    SECTION("test 1") {
        Clause *c1 = new Clause(ClauseType::WITH, assign, prog_line);
        Clause *c2 = new Clause(ClauseType::WITH, assign, constant);
        expected.addReturnReference(&assign);
        expected.addClause(c1);
        expected.addClause(c2);

        qp.preprocessQuery(TestQPreprocessor::WITH_1, actual);
        REQUIRE((actual.getClauses()[0])->equals(*c1));
        REQUIRE((actual.getClauses()[1])->equals(*c2));
        REQUIRE((actual.getReturnReferences()[0])->equals(assign));
    }

    SECTION("test 2") {
        Clause *c1 = new Clause(ClauseType::WITH, call_p, procedure);
        Clause *c2 = new Clause(ClauseType::WITH, call, constant);
        expected.addReturnReference(&call);
        expected.addClause(c1);
        expected.addClause(c2);

        qp.preprocessQuery(TestQPreprocessor::WITH_2, actual);
        REQUIRE((actual.getClauses()[0])->equals(*c1));
        REQUIRE((actual.getClauses()[1])->equals(*c2));
        REQUIRE((actual.getReturnReferences()[0])->equals(call));
    }
}

TEST_CASE("QueryPreprocessor: Select BOOLEAN") {
    const string SELECT_BOOLEAN = "Select BOOLEAN";

    QueryPreprocessor qp;
    Query q;

    qp.preprocessQuery(SELECT_BOOLEAN, q);

    REQUIRE(q.getReturnReferences().empty());
    REQUIRE(q.getClauses().empty());
}
