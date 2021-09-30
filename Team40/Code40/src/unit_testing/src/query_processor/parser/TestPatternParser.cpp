//#include "catch.hpp"
//
//#include "query_processor/Abstractions.h"
//#include "query_processor/parser/PatternParser.h"
//
//struct TestPatternParser {
//    static Reference DECLARED_STMT;
//    static Reference DECLARED_ASSIGN;
//    static Reference DECLARED_VARIABLE;
//    static Reference DECLARED_PROCEDURE;
//    static Reference WILDCARD;
//    static Reference CONSTANT_STMT_1;
//    static Reference CONSTANT_STMT_4;
//}
//
//Reference TestPatternParser::DECLARED_STMT =
//    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "statement");
//Reference TestPatternParser::DECLARED_ASSIGN =
//    Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
//Reference TestPatternParser::DECLARED_VARIABLE =
//    Reference(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "food");
//Reference TestPatternParser::DECLARED_PROCEDURE =
//    Reference(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM, "PROCEDURE");
//Reference TestPatternParser::WILDCARD =
//    Reference(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
//Reference TestPatternParser::CONSTANT_STMT_1 =
//    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
//Reference TestPatternParser::CONSTANT_STMT_4 =
//    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
//
//TEST_CASE("PatternParser: parse pattern clauses") {
//    vector<Reference *> refs = TestQueryParser::createReferenceV(2);
//
//    SECTION("test pass: constants/wildcards") {
//        PatternClause *expected = new PatternClause(
//            TestQueryParser::ASSIGN, TestQueryParser::WILDCARD_V, "_");
//        PatternClause *actual =
//            parser.parsePatternClause(make_pair("a", vector<string>{"_", "_"}));
//        REQUIRE(actual->equals(*expected));
//
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test pass: synonym declared") {
//        PatternClause *expected = new PatternClause(
//            TestQueryParser::ASSIGN, TestQueryParser::WILDCARD_V, "_");
//        PatternClause *actual =
//            parser.parsePatternClause(make_pair("a", vector<string>{"_", "_"}));
//        REQUIRE(actual->equals(*expected));
//
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test fail: synonym undeclared/invalid") {
//        REQUIRE_THROWS_AS(parser.parsePatternClause(make_pair(
//                              "a1", vector<string>{"\"x\"", "\"x\""})),
//                          ValidityError);
//        REQUIRE_THROWS_AS(parser.parsePatternClause(
//                              make_pair("s", vector<string>{"\"x\"", "\"x\""})),
//                          ValidityError);
//    }
//}
