#include "catch.hpp"

#include "query_processor/Abstractions.h"
#include "query_processor/QueryParser.h"

struct TestQueryParser {
    static const DeclPair DECL;
    static const ClsTuple REL;
    static Reference DECLARED_SYN;
    static const Reference UNDECLARED_SYN;
    static const Reference WILDCARD_S;
    static const Reference WILDCARD_V;
    static const Reference CONSTANT1;
    static const Reference CONSTANT4;
    static Reference ASSIGN;
    static vector<Reference *> createReferenceV(int n);
};

const DeclPair TestQueryParser::DECL = make_pair("stmt", "s");
const ClsTuple TestQueryParser::REL = make_tuple("Follows*", "s", "4");
Reference TestQueryParser::DECLARED_SYN =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
const Reference TestQueryParser::UNDECLARED_SYN =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "undeclared");
const Reference TestQueryParser::WILDCARD_S =
    Reference(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
const Reference TestQueryParser::WILDCARD_V =
    Reference(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
const Reference TestQueryParser::CONSTANT1 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
const Reference TestQueryParser::CONSTANT4 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
Reference TestQueryParser::ASSIGN =
    Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");

vector<Reference *> TestQueryParser::createReferenceV(int n) {
    vector<Reference *> vect;
    if (n == 1) {
        vect.push_back(&TestQueryParser::DECLARED_SYN);
    } else if (n == 2) {
        vect.push_back(&TestQueryParser::DECLARED_SYN);
        vect.push_back(&TestQueryParser::ASSIGN);
    }
    return vect;
}

//TEST_CASE("QueryParser: parseDeclaration") {
//    QueryParser parser;
//    Reference expected = TestQueryParser::DECLARED_SYN;
//    Reference *actual;
//    actual = parser.parseDeclaration(TestQueryParser::DECL);
//
//    REQUIRE(actual->getDeType() == expected.getDeType());
//    REQUIRE(actual->getRefType() == expected.getRefType());
//    REQUIRE(actual->getValue() == expected.getValue());
//    REQUIRE(actual->getDeType() == DesignEntityType::STMT);
//}
//
//TEST_CASE("QueryParser: parseSuchThatClause") {
//    QueryParser parser;
//    vector<Reference *> refs = TestQueryParser::createReferenceV(1);
//
//    SECTION("test pass: constants/wildcards") {
//        Clause *expected =
//            new Clause(ClauseType::FOLLOWS_T, TestQueryParser::CONSTANT1,
//                       TestQueryParser::CONSTANT4);
//        Clause *actual =
//            parser.parseSuchThatClause(make_tuple("Follows*", "1", "4"));
//        REQUIRE(actual->equals(*expected));
//
//        delete expected;
//        delete actual;
//
//        expected = new Clause(ClauseType::FOLLOWS_T, TestQueryParser::CONSTANT1,
//                              TestQueryParser::WILDCARD_S);
//        actual = parser.parseSuchThatClause(make_tuple("Follows*", "1", "_"));
//        REQUIRE(actual->equals(*expected));
//
//        delete expected;
//        delete actual;
//
//        expected =
//            new Clause(ClauseType::FOLLOWS_T, TestQueryParser::WILDCARD_S,
//                       TestQueryParser::CONSTANT4);
//        actual = parser.parseSuchThatClause(make_tuple("Follows*", "_", "4"));
//        REQUIRE(actual->equals(*expected));
//
//        delete expected;
//        delete actual;
//
//        expected =
//            new Clause(ClauseType::FOLLOWS_T, TestQueryParser::WILDCARD_S,
//                       TestQueryParser::WILDCARD_S);
//        actual = parser.parseSuchThatClause(make_tuple("Follows*", "_", "_"));
//        REQUIRE(actual->equals(*expected));
//
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test pass: synonym declared") {
//        Clause *expected =
//            new Clause(ClauseType::FOLLOWS_T, TestQueryParser::DECLARED_SYN,
//                       TestQueryParser::CONSTANT4);
//        Clause *actual = parser.parseSuchThatClause(TestQueryParser::REL);
//
//        REQUIRE(actual->getType() == ClauseType::FOLLOWS_T);
//        REQUIRE(actual->equals(*expected));
//
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test fail: synonym undeclared") {
//        REQUIRE_THROWS(parser.parseSuchThatClause(
//            make_tuple("Follows*", "undeclared", "4")));
//        REQUIRE_THROWS(parser.parseSuchThatClause(
//            make_tuple("Follows*", "4", "undeclared")));
//    }
//}
//
