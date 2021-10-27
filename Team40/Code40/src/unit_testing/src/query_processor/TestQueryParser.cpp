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

TEST_CASE("QueryParser: isValidName") {

    SECTION("test fail: parse invalid name") {
        string invalid = "stmt 0PD;";
        REQUIRE(!ParserUtil::isValidName(invalid));

        invalid = "assign s_fishy;";
        REQUIRE(!ParserUtil::isValidName(invalid));

        invalid = "print 11111;";
        REQUIRE(!ParserUtil::isValidName(invalid));

        invalid = " ";
        REQUIRE(!ParserUtil::isValidName(invalid));

        invalid = "";
        REQUIRE(!ParserUtil::isValidName(invalid));
    }
}