//#include "catch.hpp"
//
//#include "DesignEntityTypeHelper.h"
//
//TEST_CASE("DesignEntityTypeHelper") {
//	DesignEntityTypeHelper helper;
//
//	SECTION("test type to string map") {
//		REQUIRE(helper.getType("stmt") == DesignEntityType::STMT);
//		REQUIRE(helper.getType("assign") == DesignEntityType::ASSIGN);
//		REQUIRE_THROWS_WITH(helper.getType("nonexistant"), "invalid design entity type");
//	}
//	
//	SECTION("test string to type map") {
//		REQUIRE(helper.getValue(DesignEntityType::IF) == "if");
//	}
//}
