#include "catch.hpp"

#include "Abstractions.h"
#include "QueryParser.h"

struct TestQueryParser {
    static const DeclPair DECL;
    static const RelTuple REL;
    static const Reference REFERENCE;
    static Relation createRelation();
    static vector<Reference> createReferenceV();
};

const DeclPair TestQueryParser::DECL = make_pair("stmt", "s");
const RelTuple TestQueryParser::REL = make_tuple("Follows*", "s", "4");
const Reference TestQueryParser::REFERENCE =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");

vector<Reference> TestQueryParser::createReferenceV() {
    return vector<Reference>{REFERENCE};
}

Relation TestQueryParser::createRelation() {
    Reference r1 = TestQueryParser::REFERENCE;
    Reference r2 = Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    return Relation(RelationType::FOLLOWS_T, r1, r2);
}

TEST_CASE("QP-QueryParser: parseDeclaration") {
    QueryParser parser;
    Reference expected = TestQueryParser::REFERENCE;
    Reference actual = parser.parseDeclaration(TestQueryParser::DECL);

    REQUIRE(actual.getDeType() == expected.getDeType());
    REQUIRE(actual.getRefType() == expected.getRefType());
    REQUIRE(actual.getValue() == expected.getValue());
    REQUIRE(actual.getDeType() == DesignEntityType::STMT);
}

TEST_CASE("QP-QueryParser: parseRelation") {
    QueryParser parser;

    Relation expected = TestQueryParser::createRelation();
    Relation actual = parser.parseRelation(TestQueryParser::REL, 
        TestQueryParser::createReferenceV());

    REQUIRE(actual.getType() == RelationType::FOLLOWS_T);
    REQUIRE(actual.getType() == expected.getType());
    REQUIRE(actual.getFirstReference()->getValue() ==
            expected.getFirstReference()->getValue());
    REQUIRE((actual.getSecondReference())->getValue() ==
            expected.getSecondReference()->getValue());    
}
