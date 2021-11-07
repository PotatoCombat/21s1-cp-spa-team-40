#include "catch.hpp"

#include "query_processor/parser/WithParser.h"

TEST_CASE("WithParser: parse with clause") {
    WithParser p;
    ReferenceType syn = ReferenceType::SYNONYM;
    ReferenceType c = ReferenceType::CONSTANT;
    ReferenceAttribute aI = ReferenceAttribute::INTEGER;
    ReferenceAttribute aN = ReferenceAttribute::NAME;
    Reference D_PROG_LINE(DesignEntityType::PROG_LINE, syn, "n", aI);
    Reference D_PROG_LINE_C(DesignEntityType::STMT, syn, "n", aI);
    Reference D_STMT(DesignEntityType::STMT, syn, "s", aI);
    Reference D_VARIABLE(DesignEntityType::VARIABLE, syn, "v", aN);
    Reference D_READ_INT(DesignEntityType::READ, syn, "r", aI);
    Reference D_READ_NAME(DesignEntityType::READ, syn, "r", aN);
    Reference C_VARIABLE(DesignEntityType::VARIABLE, c, "var", aN);
    Reference C_STMT(DesignEntityType::STMT, c, "10", aI);
    vector<Reference *> declList{&D_PROG_LINE, &D_STMT, &D_VARIABLE,
                                 &D_READ_INT};
    p.initReferences(declList);

    SECTION("PASS: prog_line = constant") {
        WithPair pair = make_pair("n", "10");
        Clause expected = Clause(ClauseType::WITH, D_PROG_LINE_C, C_STMT);
        Clause *actual = p.parse(pair);

        REQUIRE(actual->equals(expected));
        delete actual;

        pair = make_pair("10", "n");
        expected = Clause(ClauseType::WITH, C_STMT, D_PROG_LINE_C);
        actual = p.parse(pair);
        REQUIRE(actual->equals(expected));
        delete actual;
    }

    SECTION("FAIL: non-prog_line-syn = constant") {
        WithPair pair = make_pair("s", "10");
        REQUIRE_THROWS_AS(p.parse(pair), ValidityError);

        pair = make_pair("10", "s");
        REQUIRE_THROWS_AS(p.parse(pair), ValidityError);
    }

    SECTION("PASS: syn.attr = constant") {
        WithPair pair = make_pair("s.stmt#", "10");
        Clause expected = Clause(ClauseType::WITH, D_STMT, C_STMT);
        Clause *actual = p.parse(pair);

        REQUIRE(actual->equals(expected));
        delete actual;

        pair = make_pair("10", "s.stmt#");
        expected = Clause(ClauseType::WITH, C_STMT, D_STMT);
        actual = p.parse(pair);
        REQUIRE(actual->equals(expected));
        delete actual;

        pair = make_pair("v.varName", "\"var\"");
        expected = Clause(ClauseType::WITH, D_VARIABLE, C_VARIABLE);
        actual = p.parse(pair);
        REQUIRE(actual->equals(expected));
        delete actual;

        pair = make_pair("\"var\"", "v.varName");
        expected = Clause(ClauseType::WITH, C_VARIABLE, D_VARIABLE);
        actual = p.parse(pair);
        REQUIRE(actual->equals(expected));
        delete actual;

        pair = make_pair("r.varName", "\"var\"");
        expected = Clause(ClauseType::WITH, D_READ_NAME, C_VARIABLE);
        actual = p.parse(pair);
        REQUIRE(actual->equals(expected));
        delete actual;

        pair = make_pair("\"var\"", "r.varName");
        expected = Clause(ClauseType::WITH, C_VARIABLE, D_READ_NAME);
        actual = p.parse(pair);
        REQUIRE(actual->equals(expected));
        delete actual;
    }

    SECTION("PASS: syn.attr = syn.attr") {
        WithPair pair = make_pair("s.stmt#", "r.stmt#");
        Clause expected = Clause(ClauseType::WITH, D_STMT, D_READ_INT);
        Clause *actual = p.parse(pair);

        REQUIRE(actual->equals(expected));
        delete actual;

        pair = make_pair("v.varName", "r.varName");
        expected = Clause(ClauseType::WITH, D_VARIABLE, D_READ_NAME);
        actual = p.parse(pair);
        REQUIRE(actual->equals(expected));
        delete actual;
    }

    SECTION("FAIL: different data types") {
        WithPair pair = make_pair("v.varName", "10");
        REQUIRE_THROWS_AS(p.parse(pair), ValidityError);

        pair = make_pair("v.varName", "prog_line");
        REQUIRE_THROWS_AS(p.parse(pair), ValidityError);

        pair = make_pair("v.varName", "r.stmt#");
        REQUIRE_THROWS_AS(p.parse(pair), ValidityError);

        pair = make_pair("s.stmt#", "\"var\"");
        REQUIRE_THROWS_AS(p.parse(pair), ValidityError);
    }
};
