#include "catch.hpp"

#include "query_processor/model/entity/Entity.h"
#include "query_processor/model/entity/NamedStatement.h"
#include "query_processor/model/entity/SynonymStatement.h"

using namespace std;

NamedStatement namedStatement("s1");
SynonymStatement synonymStatement("s2");
NamedStatement namedStatement2("s2");
NamedStatement namedStatement3("s1");

TEST_CASE("Entity: IsSynonymEntity") {
    REQUIRE(!namedStatement.IsSynonymEntity());
    REQUIRE(synonymStatement.IsSynonymEntity());
}

TEST_CASE("Entity: GetType") {
    REQUIRE(namedStatement.GetType() == EntityType::STATEMENT);
    REQUIRE(synonymStatement.GetType() == EntityType::STATEMENT);
}

TEST_CASE("Entity: Equals") { 
    REQUIRE(namedStatement.Equals(namedStatement3));
    REQUIRE(!namedStatement.Equals(namedStatement2));
    REQUIRE(!synonymStatement.Equals(namedStatement2));
}
