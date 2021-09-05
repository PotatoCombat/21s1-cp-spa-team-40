#include "catch.hpp"

#include "query_processor/query_preprocessor/pql_model/Declaration.h"

struct TestDeclaration {
	static const string SYN;
	static Declaration createStmtDeclaration();
	static Declaration createVariableDeclaration();
};

const string TestDeclaration::SYN = "s";

Declaration TestDeclaration::createStmtDeclaration() {
	return Declaration(SYN, DesignEntityType::STMT);
}

Declaration TestDeclaration::createVariableDeclaration() {
	return Declaration(SYN, DesignEntityType::VARIABLE);
}

TEST_CASE("QP-Declaration: ctor, getSynonym, getType") {
	Declaration stmt = TestDeclaration::createStmtDeclaration();
	Declaration variable = TestDeclaration::createVariableDeclaration();

	SECTION("test getType") {
		REQUIRE(stmt.getType() == DesignEntityType::STMT);
		REQUIRE(variable.getType() == DesignEntityType::VARIABLE);
	}

	SECTION("test getSynonym") {
		REQUIRE(stmt.getSynonym() == TestDeclaration::SYN);
		REQUIRE(variable.getSynonym() == TestDeclaration::SYN);
	}
}
