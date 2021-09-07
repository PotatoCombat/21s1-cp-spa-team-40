#include "catch.hpp"

#include "Entity.h"
#include "NamedStatement.h"
#include "SynonymStatement.h"

using namespace std;

NamedStatement namedStatement("s1");
SynonymStatement synonymStatement("s2");
NamedStatement namedStatement2("s2");
NamedStatement namedStatement3("s1");

TEST_CASE("Entity: IsSynonymEntity") {
    REQUIRE(!namedStatement.isSynonymEntity());
    REQUIRE(synonymStatement.isSynonymEntity());
}

TEST_CASE("Entity: GetType") {
    REQUIRE(namedStatement.getType() == EntityType::STATEMENT);
    REQUIRE(synonymStatement.getType() == EntityType::STATEMENT);
}

TEST_CASE("Entity: Equals") { 
    REQUIRE(namedStatement.equals(namedStatement3));
    REQUIRE(!namedStatement.equals(namedStatement2));
    REQUIRE(!synonymStatement.equals(namedStatement2));
}
