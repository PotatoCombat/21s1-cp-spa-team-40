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
    vector<string> PATTERN_WILDCARD{"_"};
    vector<string> PATTERN_WILDCARD_2{"_", "_"};
    vector<string> PATTERN_QUOTED{"\"x + y\""};
    vector<string> PATTERN_UNDERSCORE_QUOTED{"_\"x + y\"_"};
    vector<Reference *> declarations{&D_ASSIGN, &D_WHILE, &D_IF, &D_VARIABLE,
                                     &DECLARED_CALL};
    p.initReferences(declarations);

    SECTION("wildcard") {
        Clause *expected =
            new Clause(D_ASSIGN, WILDCARD, PATTERN_WILDCARD, true);
        Clause *actual = p.parse(make_tuple("a", "_", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_WHILE, WILDCARD);
        actual = p.parse(make_tuple("w", "_", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_IF, WILDCARD);
        actual = p.parse(make_tuple("ifs", "_", PATTERN_WILDCARD_2));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("wildcard, ?") {
        Clause *expected = new Clause(D_ASSIGN, WILDCARD, PATTERN_QUOTED, true);
        Clause *actual = p.parse(make_tuple("a", "_", PATTERN_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected =
            new Clause(D_ASSIGN, WILDCARD, PATTERN_UNDERSCORE_QUOTED, false);
        actual = p.parse(make_tuple("a", "_", PATTERN_UNDERSCORE_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("constant, ?") {
        Clause *expected =
            new Clause(D_ASSIGN, C_VARIABLE, PATTERN_WILDCARD, true);
        Clause *actual =
            p.parse(make_tuple("a", "\"const\"", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_ASSIGN, C_VARIABLE, PATTERN_QUOTED, true);
        actual = p.parse(make_tuple("a", "\"const\"", PATTERN_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected =
            new Clause(D_ASSIGN, C_VARIABLE, PATTERN_UNDERSCORE_QUOTED, false);
        actual =
            p.parse(make_tuple("a", "\"const\"", PATTERN_UNDERSCORE_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_WHILE, C_VARIABLE);
        actual = p.parse(make_tuple("w", "\"const\"", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_IF, C_VARIABLE);
        actual = p.parse(make_tuple("ifs", "\"const\"", PATTERN_WILDCARD_2));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("synonym, ?") {
        Clause *expected =
            new Clause(D_ASSIGN, D_VARIABLE, PATTERN_WILDCARD, true);
        Clause *actual = p.parse(make_tuple("a", "v", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_ASSIGN, D_VARIABLE, PATTERN_QUOTED, true);
        actual = p.parse(make_tuple("a", "v", PATTERN_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected =
            new Clause(D_ASSIGN, D_VARIABLE, PATTERN_UNDERSCORE_QUOTED, false);
        actual = p.parse(make_tuple("a", "v", PATTERN_UNDERSCORE_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_WHILE, D_VARIABLE);
        actual = p.parse(make_tuple("w", "v", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_IF, D_VARIABLE);
        actual = p.parse(make_tuple("ifs", "v", PATTERN_WILDCARD_2));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("TEST FAIL: stmt undeclared/invalid") {
        REQUIRE_THROWS_AS(p.parse(make_tuple("a1", "\"x\"", PATTERN_QUOTED)),
                          ValidityError);
        REQUIRE_THROWS_AS(p.parse(make_tuple("c", "\"x\"", PATTERN_QUOTED)),
                          ValidityError);
        REQUIRE_THROWS_AS(p.parse(make_tuple("a", "var", PATTERN_QUOTED)),
                          ValidityError);
        REQUIRE_THROWS_AS(p.parse(make_tuple("a", "c", PATTERN_QUOTED)),
                          ValidityError);
    }

    SECTION("TEST FAIL: invalid var/arg to assign") {
        REQUIRE_THROWS_AS(p.parse(make_tuple("a", "_\"x\"_", PATTERN_QUOTED)),
                          ValidityError);
        REQUIRE_THROWS_AS(
            p.parse(make_tuple("a", "_", vector<string>{"_\"x\""})),
            ValidityError);
        REQUIRE_THROWS_AS(p.parse(make_tuple("a", "_", vector<string>{"\"x"})),
                          ValidityError);
    }

    SECTION("TEST FAIL: invalid second (and third) arguments to while/if") {
        vector<string> PATTERN_MULTI_1{"_", "\"x\""};
        vector<string> PATTERN_MULTI_2{"\"x\"", "_"};
        REQUIRE_THROWS_AS(p.parse(make_tuple("w", "\"x\"", PATTERN_QUOTED)),
                          ValidityError);
        REQUIRE_THROWS_AS(p.parse(make_tuple("ifs", "\"x\"", PATTERN_MULTI_1)),
                          ValidityError);
        REQUIRE_THROWS_AS(p.parse(make_tuple("ifs", "\"x\"", PATTERN_MULTI_2)),
                          ValidityError);
    }
}
