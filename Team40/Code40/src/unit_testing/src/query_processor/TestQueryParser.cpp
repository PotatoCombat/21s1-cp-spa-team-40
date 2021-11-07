#include "catch.hpp"

#include "query_processor/QueryParser.h"

TEST_CASE("QueryParser: parseDeclarations valid") {
    vector<DeclPair> pairs;
    pairs.push_back(make_pair("stmt", "s"));
    pairs.push_back(make_pair("assign", "a"));
    pairs.push_back(make_pair("assign", "a1"));
    pairs.push_back(make_pair("call", "c"));
    pairs.push_back(make_pair("print", "p"));
    pairs.push_back(make_pair("read", "r"));
    pairs.push_back(make_pair("while", "w"));
    pairs.push_back(make_pair("if", "i"));
    pairs.push_back(make_pair("prog_line", "n"));
    pairs.push_back(make_pair("variable", "v"));
    pairs.push_back(make_pair("procedure", "proc"));
    pairs.push_back(make_pair("constant", "const"));

    ReferenceType refType = ReferenceType::SYNONYM;
    ReferenceAttribute attrInt = ReferenceAttribute::INTEGER;
    ReferenceAttribute attrName = ReferenceAttribute::NAME;

    vector<Reference *> refs;
    refs.push_back(
        new Reference(DesignEntityType::STMT, refType, "s", attrInt));
    refs.push_back(
        new Reference(DesignEntityType::ASSIGN, refType, "a", attrInt));
    refs.push_back(
        new Reference(DesignEntityType::ASSIGN, refType, "a1", attrInt));
    refs.push_back(
        new Reference(DesignEntityType::CALL, refType, "c", attrInt));
    refs.push_back(
        new Reference(DesignEntityType::PRINT, refType, "p", attrInt));
    refs.push_back(
        new Reference(DesignEntityType::READ, refType, "r", attrInt));
    refs.push_back(
        new Reference(DesignEntityType::WHILE, refType, "w", attrInt));
    refs.push_back(new Reference(DesignEntityType::IF, refType, "i", attrInt));
    refs.push_back(
        new Reference(DesignEntityType::PROG_LINE, refType, "n", attrInt));
    refs.push_back(
        new Reference(DesignEntityType::VARIABLE, refType, "v", attrName));
    refs.push_back(
        new Reference(DesignEntityType::PROCEDURE, refType, "proc", attrName));
    refs.push_back(
        new Reference(DesignEntityType::CONSTANT, refType, "const", attrInt));

    QueryParser parser;

    parser.parseDeclarations(pairs);

    for (size_t i = 0; i < pairs.size(); ++i) {
        Reference *expected = refs[i];
        Reference *actual = parser.getReferenceIfDeclared(pairs[i].second);
        REQUIRE(actual->equals(*expected));
        REQUIRE(actual->getAttr() == expected->getAttr());
    }
}

TEST_CASE("QueryParser: parseDeclarations invalid") {
    QueryParser parser;

    SECTION("FAIL: same synonym declared twice") {
        vector<DeclPair> pairs;
        pairs.push_back(make_pair("stmt", "s"));
        pairs.push_back(make_pair("assign", "s"));
        REQUIRE_THROWS_AS(parser.parseDeclarations(pairs), ValidityError);
    }
}
