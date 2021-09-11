#include "catch.hpp"

#include "query_processor/Abstractions.h"
#include "query_processor/QueryParser.h"

struct TestQueryParser {
    static const DeclPair DECL;
    static const ClsTuple REL;
    static const Reference REFERENCE;
    static Clause createClause();
    static vector<Reference*> createReferenceV();
};

const DeclPair TestQueryParser::DECL = make_pair("stmt", "s");
const ClsTuple TestQueryParser::REL = make_tuple("Follows*", "s", "4");
const Reference TestQueryParser::REFERENCE =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");

vector<Reference*> TestQueryParser::createReferenceV() {
    return vector<Reference*>{new Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s")};
}

Clause TestQueryParser::createClause() {
    Reference r1 = TestQueryParser::REFERENCE;
    Reference r2 = Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    return Clause(ClauseType::FOLLOWS_T, r1, r2);
}

TEST_CASE("QueryParser: parseDeclaration") {
    QueryParser parser;
    Reference expected = TestQueryParser::REFERENCE;
    Reference *actual;
    actual = parser.parseDeclaration(TestQueryParser::DECL);

    REQUIRE(actual->getDeType() == expected.getDeType());
    REQUIRE(actual->getRefType() == expected.getRefType());
    REQUIRE(actual->getValue() == expected.getValue());
    REQUIRE(actual->getDeType() == DesignEntityType::STMT);
}

TEST_CASE("QueryParser: parseRelation") {
    QueryParser parser;

    Clause expected = TestQueryParser::createClause();
    Clause *actual = parser.parseClause(TestQueryParser::REL, 
        TestQueryParser::createReferenceV());

    REQUIRE(actual->getType() == ClauseType::FOLLOWS_T);
    REQUIRE(actual->getType() == expected.getType());
    REQUIRE(actual->getFirstReference()->getValue() ==
            expected.getFirstReference()->getValue());
    REQUIRE((actual->getSecondReference())->getValue() ==
            expected.getSecondReference()->getValue());    
}
