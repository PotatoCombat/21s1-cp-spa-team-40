#include "catch.hpp"

#include <cstdarg>

#include "query_processor/Abstractions.h"
#include "query_processor/QueryParser.h"

struct TestQueryParser {
    static const DeclPair DECL;
    static const ClsTuple REL;
    static const Reference DECLARED_SYN;
    static const Reference UNDECLARED_SYN;
    static const Reference WILDCARD;
    static const Reference CONSTANT1;
    static const Reference CONSTANT4;
    static Clause *createClauseSynConst();
    static vector<Reference *> createReferenceV(int n);
};

const DeclPair TestQueryParser::DECL = make_pair("stmt", "s");
const ClsTuple TestQueryParser::REL = make_tuple("Follows*", "s", "4");
const Reference TestQueryParser::DECLARED_SYN =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
const Reference TestQueryParser::UNDECLARED_SYN =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "undeclared");
const Reference TestQueryParser::WILDCARD =
    Reference(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
const Reference TestQueryParser::CONSTANT1 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
const Reference TestQueryParser::CONSTANT4 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");

vector<Reference *> TestQueryParser::createReferenceV(int n) {
    vector<Reference *> vect;
    if (n == 1) {
        vect.push_back(
            new Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s"));
    }
    return vect;
}

Clause *TestQueryParser::createClauseSynConst() {
    Reference r1 = TestQueryParser::DECLARED_SYN;
    Reference r2 =
        Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    return new Clause(ClauseType::FOLLOWS_T, r1, r2);
}

TEST_CASE("QueryParser: parseDeclaration") {
    QueryParser parser;
    Reference expected = TestQueryParser::DECLARED_SYN;
    Reference *actual;
    actual = parser.parseDeclaration(TestQueryParser::DECL);

    REQUIRE(actual->getDeType() == expected.getDeType());
    REQUIRE(actual->getRefType() == expected.getRefType());
    REQUIRE(actual->getValue() == expected.getValue());
    REQUIRE(actual->getDeType() == DesignEntityType::STMT);
}

TEST_CASE("QueryParser: parseRelation") {
    QueryParser parser;
    vector<Reference *> refs = TestQueryParser::createReferenceV(1);

    SECTION("test pass: constants/wildcards") {
        //Clause *expected =
        //    new Clause(ClauseType::FOLLOWS_T, TestQueryParser::CONSTANT1,
        //               TestQueryParser::CONSTANT4);
        //Clause *actual =
        //    parser.parseClause(make_tuple("Follows*", "1", "4"), refs);
        //REQUIRE(actual->equals(*expected));

        //expected =
        //    new Clause(ClauseType::FOLLOWS_T, TestQueryParser::CONSTANT1,
        //               TestQueryParser::WILDCARD);
        //actual = parser.parseClause(make_tuple("Follows*", "1", "_"), refs);
        //REQUIRE(actual->equals(*expected));

        //expected = new Clause(ClauseType::FOLLOWS_T, TestQueryParser::WILDCARD,
        //                      TestQueryParser::CONSTANT4);
        //actual = parser.parseClause(make_tuple("Follows*", "_", "4"), refs);
        //REQUIRE(actual->equals(*expected));

        //expected = new Clause(ClauseType::FOLLOWS_T, TestQueryParser::WILDCARD,
        //                      TestQueryParser::WILDCARD);
        //actual = parser.parseClause(make_tuple("Follows*", "_", "_"), refs);
        //REQUIRE(actual->equals(*expected));
    }

    SECTION("test pass: synonym declared") {
        Clause *expected = TestQueryParser::createClauseSynConst();
        Clause *actual = parser.parseClause(TestQueryParser::REL, refs);

        REQUIRE(actual->getType() == ClauseType::FOLLOWS_T);
        REQUIRE(actual->getType() == expected->getType());
        REQUIRE(actual->getFirstReference()->getValue() ==
                expected->getFirstReference()->getValue());
        REQUIRE(actual->getSecondReference()->getValue() ==
                expected->getSecondReference()->getValue());
    }

    SECTION("test fail: synonym undeclared") {
        //Clause *expected =
        //    new Clause(ClauseType::FOLLOWS_T, TestQueryParser::UNDECLARED_SYN,
        //               TestQueryParser::CONSTANT4);

        REQUIRE_THROWS(parser.parseClause(make_tuple("Follows*", "undeclared", "4"), refs));
    }
}
