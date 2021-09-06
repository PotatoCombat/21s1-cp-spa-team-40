#include "catch.hpp"

#include "query_processor/query_preprocessor/QueryParser.h"

struct TestQueryParser {
    static const pair<string, string> DECL;
    static const tuple<string, string, string> STCL;
    static Declaration createDeclaration();
    static SuchThatClause createSuchThatClause();
    // static PatternClause createPatternClause();
};

const pair<string, string> TestQueryParser::DECL = make_pair("stmt", "s");
const tuple<string, string, string> TestQueryParser::STCL =
    make_tuple("Follows", "s", "p1");

Declaration TestQueryParser::createDeclaration() {
    return Declaration(DECL.second, DesignEntityType::STMT);
}

SuchThatClause TestQueryParser::createSuchThatClause() {
    return SuchThatClause(
        Relation(get<1>(STCL), get<2>(STCL), RelationType::FOLLOWS));
}

TEST_CASE("QP-QueryParser: parseDeclaration") {
    QueryParser parser;
    Declaration expected = TestQueryParser::createDeclaration();
    Declaration actual = parser.parseDeclaration(TestQueryParser::DECL);

    REQUIRE(expected.getSynonym() == actual.getSynonym());
    REQUIRE(expected.getType() == actual.getType());
    REQUIRE(expected.getType() == DesignEntityType::STMT);
}

TEST_CASE("QP-QueryParser: parseSuchThatClause") {
    QueryParser parser;
    SuchThatClause expected = TestQueryParser::createSuchThatClause();
    SuchThatClause actual = parser.parseSuchThatClause(TestQueryParser::STCL);

    REQUIRE(expected.getRelation().getFirstReference().getValue() ==
            actual.getRelation().getFirstReference().getValue());
    REQUIRE(expected.getRelation().getSecondReference().getValue() ==
            actual.getRelation().getSecondReference().getValue());
    REQUIRE(expected.getRelation().getRelationType() ==
            actual.getRelation().getRelationType());
    REQUIRE(expected.getRelation().getRelationType() == RelationType::FOLLOWS);
}
