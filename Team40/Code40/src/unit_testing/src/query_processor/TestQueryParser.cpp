#include "catch.hpp"

#include "QueryParser.h"
#include "Abstractions.h"

struct TestQueryParser {
    static const DeclPair DECL;
    static const RelTuple REL;
    static Reference createReference();
    static Relation createRelation();
};

const DeclPair TestQueryParser::DECL = make_pair("stmt", "s");
const RelTuple TestQueryParser::REL = make_tuple("Follows*", "s", "4");

Reference TestQueryParser::createReference() {
    return Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
}

Relation TestQueryParser::createRelation() {
    Reference r1 = createReference();
    Reference r2(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    return Relation(RelationType::FOLLOWS, &r1, &r2);
}

TEST_CASE("QP-QueryParser: parseDeclaration") {
    QueryParser parser;
    Reference expected = TestQueryParser::createReference();
    Reference actual = parser.parseDeclaration(TestQueryParser::DECL);

    REQUIRE(actual.getDeType() == expected.getDeType());
    REQUIRE(actual.getRefType() == expected.getRefType());
    REQUIRE(actual.getValue() == expected.getValue());
    REQUIRE(actual.getDeType() == DesignEntityType::STMT);
}

//TEST_CASE("QP-QueryParser: parseSuchThatClause") {
//    QueryParser parser;
//    SuchThatClause expected = TestQueryParser::createSuchThatClause();
//    SuchThatClause actual = parser.parseSuchThatClause(TestQueryParser::STCL);
//
//    REQUIRE(expected.getRelation().getFirstReference().getValue() ==
//            actual.getRelation().getFirstReference().getValue());
//    REQUIRE(expected.getRelation().getSecondReference().getValue() ==
//            actual.getRelation().getSecondReference().getValue());
//    REQUIRE(expected.getRelation().getRelationType() ==
//            actual.getRelation().getRelationType());
//    REQUIRE(expected.getRelation().getRelationType() == RelationType::FOLLOWS);
//}
