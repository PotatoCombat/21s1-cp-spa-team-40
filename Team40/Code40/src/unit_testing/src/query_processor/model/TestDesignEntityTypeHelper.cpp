#include "catch.hpp"

#include "query_processor/model/DesignEntityTypeHelper.h"

TEST_CASE("DesignEntityTypeHelper") {
	DesignEntityTypeHelper helper;

	SECTION("test string to type map") {
		REQUIRE(helper.valueToDesType("stmt") == DesignEntityType::STMT);
		REQUIRE(helper.valueToDesType("assign") == DesignEntityType::ASSIGN);
		REQUIRE_THROWS_WITH(helper.valueToDesType("nonexistant"), "invalid design entity type");
	}

}
