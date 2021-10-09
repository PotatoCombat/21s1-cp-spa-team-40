#include "catch.hpp"

#include "query_processor/Abstractions.h"
#include "query_processor/parser/PatternParser.h"

TEST_CASE("PatternParser: parse pattern clauses") {
    PatternParser p;
    ReferenceType refT = ReferenceType::SYNONYM;
    DesignEntityType var = DesignEntityType::VARIABLE;
    Reference D_ASSIGN = Reference(DesignEntityType::ASSIGN, refT, "a");
    Reference D_WHILE = Reference(DesignEntityType::WHILE, refT, "w");
    Reference D_IF = Reference(DesignEntityType::IF, refT, "ifs");
    Reference D_VARIABLE = Reference(var, refT, "v");
    Reference C_VARIABLE = Reference(var, ReferenceType::CONSTANT, "const");
    Reference WILDCARD = Reference(var, ReferenceType::WILDCARD, "_");
    Reference DECLARED_CALL = Reference(DesignEntityType::CALL, refT, "c");
    string PATTERN_WILDCARD = "_";
    string PATTERN_QUOTED = "\"x + y\"";
    string PATTERN_UNDERSCORE_QUOTED = "_\"x + y\"_";
    vector<Reference *> declarations{&D_ASSIGN, &D_WHILE, &D_IF, &D_VARIABLE,
                                     &DECLARED_CALL};
    p.initReferences(declarations);

    SECTION("wildcard") {
        PatternClause *expected =
            new PatternClause(D_ASSIGN, WILDCARD, PATTERN_WILDCARD, true);
        PatternClause *actual =
            p.parse(make_pair("a", vector<string>{"_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_WHILE, WILDCARD);
        actual = p.parse(make_pair("w", vector<string>{"_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_IF, WILDCARD);
        actual = p.parse(make_pair("ifs", vector<string>{"_", "_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("wildcard, ?") {
        PatternClause *expected =
            new PatternClause(D_ASSIGN, WILDCARD, PATTERN_QUOTED, true);
        PatternClause *actual =
            p.parse(make_pair("a", vector<string>{"_", PATTERN_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_ASSIGN, WILDCARD,
                                     PATTERN_UNDERSCORE_QUOTED, false);
        actual = p.parse(
            make_pair("a", vector<string>{"_", PATTERN_UNDERSCORE_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("constant, ?") {
        PatternClause *expected =
            new PatternClause(D_ASSIGN, C_VARIABLE, PATTERN_WILDCARD, true);
        PatternClause *actual =
            p.parse(make_pair("a", vector<string>{"\"const\"", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected =
            new PatternClause(D_ASSIGN, C_VARIABLE, PATTERN_QUOTED, true);
        actual = p.parse(
            make_pair("a", vector<string>{"\"const\"", PATTERN_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_ASSIGN, C_VARIABLE,
                                     PATTERN_UNDERSCORE_QUOTED, false);
        actual = p.parse(make_pair(
            "a", vector<string>{"\"const\"", PATTERN_UNDERSCORE_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_WHILE, C_VARIABLE);
        actual = p.parse(make_pair("w", vector<string>{"\"const\"", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_IF, C_VARIABLE);
        actual =
            p.parse(make_pair("ifs", vector<string>{"\"const\"", "_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("synonym, ?") {
        PatternClause *expected =
            new PatternClause(D_ASSIGN, D_VARIABLE, PATTERN_WILDCARD, true);
        PatternClause *actual =
            p.parse(make_pair("a", vector<string>{"v", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected =
            new PatternClause(D_ASSIGN, D_VARIABLE, PATTERN_QUOTED, true);
        actual = p.parse(make_pair("a", vector<string>{"v", PATTERN_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_ASSIGN, D_VARIABLE,
                                     PATTERN_UNDERSCORE_QUOTED, false);
        actual = p.parse(
            make_pair("a", vector<string>{"v", PATTERN_UNDERSCORE_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_WHILE, D_VARIABLE);
        actual = p.parse(make_pair("w", vector<string>{"v", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(D_IF, D_VARIABLE);
        actual = p.parse(make_pair("ifs", vector<string>{"v", "_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("TEST FAIL: synonym undeclared/invalid") {
        REQUIRE_THROWS_AS(
            p.parse(make_pair("a1", vector<string>{"\"x\"", "\"x\""})),
            ValidityError);
        REQUIRE_THROWS_AS(
            p.parse(make_pair("c", vector<string>{"\"x\"", "\"x\""})),
            ValidityError);
        REQUIRE_THROWS_AS(
            p.parse(make_pair("a", vector<string>{"var", "\"x\""})),
            ValidityError);
        REQUIRE_THROWS_AS(p.parse(make_pair("a", vector<string>{"c", "\"x\""})),
                          ValidityError);
    }

    SECTION("TEST FAIL: invalid first argument") {
        REQUIRE_THROWS_AS(
            p.parse(make_pair("a", vector<string>{"_\"x\"_", "\"x\""})),
            ValidityError);
        REQUIRE_THROWS_AS(
            p.parse(make_pair("a", vector<string>{"_", "_\"x\""})),
            ValidityError);
        REQUIRE_THROWS_AS(p.parse(make_pair("a", vector<string>{"_", "\"x"})),
                          ValidityError);
    }

    SECTION("TEST FAIL: invalid second (and third) arguments to while/if") {
        REQUIRE_THROWS_AS(
            p.parse(make_pair("w", vector<string>{"\"x\"", "\"x\""})),
            ValidityError);
        REQUIRE_THROWS_AS(
            p.parse(make_pair("ifs", vector<string>{"\"x\"", "\"x\"", "_"})),
            ValidityError);
        REQUIRE_THROWS_AS(
            p.parse(make_pair("ifs", vector<string>{"\"x\"", "_", "\"x\""})),
            ValidityError);
    }
}
