#include "catch.hpp"

#include "query_processor/Abstractions.h"
#include "query_processor/parser/PatternParser.h"

struct TestPatternParser {
    static Reference DECLARED_ASSIGN;
    static Reference DECLARED_WHILE;
    static Reference DECLARED_IF;
    static Reference DECLARED_VARIABLE;
    static Reference CONSTANT_VARIABLE;
    static Reference WILDCARD;
    static Reference DECLARED_CALL;
    static string PATTERN_WILDCARD;
    static string PATTERN_QUOTED;
    static string PATTERN_UNDERSCORE_QUOTED;
    static vector<Reference *> createReferences();
};

Reference TestPatternParser::DECLARED_ASSIGN =
    Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
Reference TestPatternParser::DECLARED_WHILE =
    Reference(DesignEntityType::WHILE, ReferenceType::SYNONYM, "w");
Reference TestPatternParser::DECLARED_IF =
    Reference(DesignEntityType::IF, ReferenceType::SYNONYM, "ifs");
Reference TestPatternParser::DECLARED_VARIABLE =
    Reference(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "v");
Reference TestPatternParser::CONSTANT_VARIABLE =
    Reference(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "constant");
Reference TestPatternParser::WILDCARD =
    Reference(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
Reference TestPatternParser::DECLARED_CALL =
    Reference(DesignEntityType::CALL, ReferenceType::SYNONYM, "c");
string TestPatternParser::PATTERN_WILDCARD = "_";
string TestPatternParser::PATTERN_QUOTED = "\"x + y\"";
string TestPatternParser::PATTERN_UNDERSCORE_QUOTED = "_\"x + y\"_";

vector<Reference *> TestPatternParser::createReferences() {
    return vector<Reference *>{&DECLARED_ASSIGN, &DECLARED_WHILE, &DECLARED_IF,
                               &DECLARED_VARIABLE, &DECLARED_CALL};
}

TEST_CASE("PatternParser: parse pattern clauses") {
    PatternParser p;
    p.initReferences(TestPatternParser::createReferences());

    SECTION("wildcard") {
        PatternClause *expected = new PatternClause(
            TestPatternParser::DECLARED_ASSIGN, TestPatternParser::WILDCARD,
            TestPatternParser::PATTERN_WILDCARD, true);
        PatternClause *actual =
            p.parse(make_pair("a", vector<string>{"_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(TestPatternParser::DECLARED_WHILE,
                                     TestPatternParser::WILDCARD);
        actual = p.parse(make_pair("w", vector<string>{"_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(TestPatternParser::DECLARED_IF,
                                     TestPatternParser::WILDCARD);
        actual = p.parse(make_pair("ifs", vector<string>{"_", "_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("wildcard, ?") {
        PatternClause *expected = new PatternClause(
            TestPatternParser::DECLARED_ASSIGN, TestPatternParser::WILDCARD,
            TestPatternParser::PATTERN_QUOTED, true);
        PatternClause *actual = p.parse(make_pair(
            "a", vector<string>{"_", TestPatternParser::PATTERN_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(
            TestPatternParser::DECLARED_ASSIGN, TestPatternParser::WILDCARD,
            TestPatternParser::PATTERN_UNDERSCORE_QUOTED, true);
        actual = p.parse(make_pair(
            "a",
            vector<string>{"_", TestPatternParser::PATTERN_UNDERSCORE_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("constant, ?") {
        PatternClause *expected =
            new PatternClause(TestPatternParser::DECLARED_ASSIGN,
                              TestPatternParser::CONSTANT_VARIABLE,
                              TestPatternParser::PATTERN_WILDCARD, true);
        PatternClause *actual =
            p.parse(make_pair("a", vector<string>{"\"constant\"", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(TestPatternParser::DECLARED_ASSIGN,
                                     TestPatternParser::CONSTANT_VARIABLE,
                                     TestPatternParser::PATTERN_QUOTED, true);
        actual = p.parse(
            make_pair("a", vector<string>{"\"constant\"",
                                          TestPatternParser::PATTERN_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(
            TestPatternParser::DECLARED_ASSIGN,
            TestPatternParser::CONSTANT_VARIABLE,
            TestPatternParser::PATTERN_UNDERSCORE_QUOTED, true);
        actual = p.parse(make_pair(
            "a", vector<string>{"\"constant\"",
                                TestPatternParser::PATTERN_UNDERSCORE_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(TestPatternParser::DECLARED_WHILE,
                                     TestPatternParser::CONSTANT_VARIABLE);
        actual = p.parse(make_pair("w", vector<string>{"\"constant\"", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(TestPatternParser::DECLARED_IF,
                                     TestPatternParser::CONSTANT_VARIABLE);
        actual =
            p.parse(make_pair("ifs", vector<string>{"\"constant\"", "_", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;
    }

    SECTION("synonym, ?") {
        PatternClause *expected =
            new PatternClause(TestPatternParser::DECLARED_ASSIGN,
                              TestPatternParser::DECLARED_VARIABLE,
                              TestPatternParser::PATTERN_WILDCARD, true);
        PatternClause *actual =
            p.parse(make_pair("a", vector<string>{"v", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(TestPatternParser::DECLARED_ASSIGN,
                                     TestPatternParser::DECLARED_VARIABLE,
                                     TestPatternParser::PATTERN_QUOTED, true);
        actual = p.parse(make_pair(
            "a", vector<string>{"v", TestPatternParser::PATTERN_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(
            TestPatternParser::DECLARED_ASSIGN,
            TestPatternParser::DECLARED_VARIABLE,
            TestPatternParser::PATTERN_UNDERSCORE_QUOTED, true);
        actual = p.parse(make_pair(
            "a",
            vector<string>{"v", TestPatternParser::PATTERN_UNDERSCORE_QUOTED}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(TestPatternParser::DECLARED_WHILE,
                                     TestPatternParser::DECLARED_VARIABLE);
        actual = p.parse(make_pair("w", vector<string>{"v", "_"}));
        REQUIRE(actual->equals(*expected));
        delete expected, actual;

        expected = new PatternClause(TestPatternParser::DECLARED_IF,
                                     TestPatternParser::DECLARED_VARIABLE);
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
