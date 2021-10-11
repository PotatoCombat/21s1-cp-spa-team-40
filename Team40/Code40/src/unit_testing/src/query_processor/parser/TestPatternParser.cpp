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
    Reference D_CALL = Reference(DesignEntityType::CALL, refT, "c");
    vector<string> EMPTY;
    vector<string> PATTERN_WILDCARD{"_"};
    vector<string> PATTERN_WILDCARD_2{"_", "_"};
    vector<string> PATTERN_QUOTED{"\"", "x", "+", "y", "\""};
    vector<string> PATTERN_UNDERSCORE_QUOTED{"_", "\"", "x", "+",
                                             "y", "\"", "_"};
    vector<string> PATTERN{"x", "+", "y"};
    vector<Reference *> declarations{&D_ASSIGN, &D_WHILE, &D_IF, &D_VARIABLE,
                                     &D_CALL};
    p.initReferences(declarations);

    SECTION("wildcard") {
        Clause *expected = new Clause(D_ASSIGN, WILDCARD, EMPTY, true);
        Clause *actual = p.parse(make_tuple("a", "_", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(ClauseType::PATTERN, D_WHILE, WILDCARD);
        actual = p.parse(make_tuple("w", "_", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(ClauseType::PATTERN, D_IF, WILDCARD);
        actual = p.parse(make_tuple("ifs", "_", PATTERN_WILDCARD_2));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("wildcard, ?") {
        Clause *expected = new Clause(D_ASSIGN, WILDCARD, PATTERN, true);
        Clause *actual = p.parse(make_tuple("a", "_", PATTERN_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_ASSIGN, WILDCARD, PATTERN, false);
        actual = p.parse(make_tuple("a", "_", PATTERN_UNDERSCORE_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("constant, ?") {
        Clause *expected = new Clause(D_ASSIGN, C_VARIABLE, EMPTY, true);
        Clause *actual =
            p.parse(make_tuple("a", "\"const\"", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_ASSIGN, C_VARIABLE, PATTERN, true);
        actual = p.parse(make_tuple("a", "\"const\"", PATTERN_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_ASSIGN, C_VARIABLE, PATTERN, false);
        actual =
            p.parse(make_tuple("a", "\"const\"", PATTERN_UNDERSCORE_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(ClauseType::PATTERN, D_WHILE, C_VARIABLE);
        actual = p.parse(make_tuple("w", "\"const\"", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(ClauseType::PATTERN, D_IF, C_VARIABLE);
        actual = p.parse(make_tuple("ifs", "\"const\"", PATTERN_WILDCARD_2));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("synonym, ?") {
        Clause *expected = new Clause(D_ASSIGN, D_VARIABLE, EMPTY, true);
        Clause *actual = p.parse(make_tuple("a", "v", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_ASSIGN, D_VARIABLE, PATTERN, true);
        actual = p.parse(make_tuple("a", "v", PATTERN_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(D_ASSIGN, D_VARIABLE, PATTERN, false);
        actual = p.parse(make_tuple("a", "v", PATTERN_UNDERSCORE_QUOTED));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(ClauseType::PATTERN, D_WHILE, D_VARIABLE);
        actual = p.parse(make_tuple("w", "v", PATTERN_WILDCARD));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new Clause(ClauseType::PATTERN, D_IF, D_VARIABLE);
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
        REQUIRE_THROWS_AS(p.parse(make_tuple("a", "c", PATTERN_QUOTED)),
                          ValidityError);
        REQUIRE_THROWS_AS(
            p.parse(make_tuple("a", "_", vector<string>{"_", "\"", "x", "\""})),
            ValidityError);
        REQUIRE_THROWS_AS(
            p.parse(make_tuple("a", "_", vector<string>{"\"", "x"})),
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

TEST_CASE("PatternParser: parse pattern string into tokens") {
    PatternParser p;

    SECTION("PASS: valid patterns") {
        vector<string> P_SIMPLE_1{"\"", "x", "+", "y", "\""};
        vector<string> P_SIMPLE_2{"\"", "x", "+", "10", "+", "y", "\""};
        vector<string> P_BRACKETS_1{"\"", "(", "(", "y", ")", ")", "\""};
        vector<string> P_BRACKETS_2{"\"", "x", "+", "(", "(", "y", ")",
                                    "*",  "(", "x", ")", ")", "\""};
        vector<string> P_COMPLEX_1{"_",  "\"", "x", "+",    "y", "-",
                                   "10", "*",  "(", "n4m3", "%", "x",
                                   ")",  "/",  "y", "\"",   "_"};
        vector<string> P_COMPLEX_2{"_",  "\"", "(",  "(", "(",  "x", "-",
                                   "y",  ")",  "*",  "z", ")",  "/", "(",
                                   "a",  "-",  "20", "%", "b",  ")", "-",
                                   "10", ")",  "*",  "x", "\"", "_"};

        vector<string> T_SIMPLE_1{"x", "+", "y"};
        vector<string> T_SIMPLE_2{"x", "+", "10", "+", "y"};
        vector<string> T_BRACKETS_1{"(", "(", "y", ")", ")"};
        vector<string> T_BRACKETS_2{"x", "+", "(", "(", "y", ")",
                                    "*", "(", "x", ")", ")"};
        vector<string> T_COMPLEX_1{"x",    "+", "y", "-", "10", "*", "(",
                                   "n4m3", "%", "x", ")", "/",  "y"};
        vector<string> T_COMPLEX_2{"(", "(", "(", "x",  "-", "y", ")",  "*",
                                   "z", ")", "/", "(",  "a", "-", "20", "%",
                                   "b", ")", "-", "10", ")", "*", "x"};

        vector<string> tokens = p.parsePatternTokens(P_SIMPLE_1);
        REQUIRE(tokens == T_SIMPLE_1);

        tokens = p.parsePatternTokens(P_SIMPLE_2);
        REQUIRE(tokens == T_SIMPLE_2);

        tokens = p.parsePatternTokens(P_BRACKETS_1);
        REQUIRE(tokens == T_BRACKETS_1);

        tokens = p.parsePatternTokens(P_BRACKETS_2);
        REQUIRE(tokens == T_BRACKETS_2);

        tokens = p.parsePatternTokens(P_COMPLEX_1);
        REQUIRE(tokens == T_COMPLEX_1);

        tokens = p.parsePatternTokens(P_COMPLEX_2);
        REQUIRE(tokens == T_COMPLEX_2);
    }

    SECTION("FAIL: mismatching brackets") {
        vector<string> P_BRACKETS_1{"(", "(", "y", ")"};
        vector<string> P_BRACKETS_2{"(", "y", ")", ")"};
        vector<string> P_BRACKETS_3{"(", "(", "y", ")", ")", "("};

        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_1), ValidityError);
        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_2), ValidityError);
        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_3), ValidityError);
    }

    SECTION("FAIL: invalid operator location") {
        vector<string> P_BRACKETS_1{"x", "+", "y", "-"};
        vector<string> P_BRACKETS_2{"x", "+", "-", "y"};
        vector<string> P_BRACKETS_3{"+", "x"};
        vector<string> P_BRACKETS_4{"+"};

        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_1), ValidityError);
        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_2), ValidityError);
        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_3), ValidityError);
        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_4), ValidityError);
    }

    SECTION("FAIL: invalid name/integer") {
        vector<string> P_BRACKETS_1{"x y"};
        vector<string> P_BRACKETS_2{"1234d"};

        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_1), ValidityError);
        REQUIRE_THROWS_AS(p.parsePatternTokens(P_BRACKETS_2), ValidityError);
    }
}
