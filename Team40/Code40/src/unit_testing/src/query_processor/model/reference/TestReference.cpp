#include "catch.hpp"

#include "Reference.h"
#include "NamedStatement.h"
#include "SynonymStatement.h"

using namespace std;

NamedStatement namedStatement("s1", DesignEntityType::STMT);
SynonymStatement synonymStatement("s2", DesignEntityType::STMT);
NamedStatement namedStatement2("s2", DesignEntityType::STMT);
NamedStatement namedStatement3("s1", DesignEntityType::STMT);

TEST_CASE("Reference: IsSynonym") {
    REQUIRE(!namedStatement.isSynonym());
    REQUIRE(synonymStatement.isSynonym());
}

TEST_CASE("Reference: GetType") {
    REQUIRE(namedStatement.getType() == DesignEntityType::STMT);
    REQUIRE(synonymStatement.getType() == DesignEntityType::STMT);
}

TEST_CASE("Reference: Equals") { 
    REQUIRE(namedStatement.equals(namedStatement3));
    REQUIRE(!namedStatement.equals(namedStatement2));
    REQUIRE(!synonymStatement.equals(namedStatement2));
}
