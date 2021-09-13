#include "catch.hpp"
#include "source_processor/design_extractor/ExtractionContext.h"

TEST_CASE("ExtractionContext: Creates correctly") {
    REQUIRE(
        !ExtractionContext::getInstance().getCurrentProcedure().has_value());
    REQUIRE(ExtractionContext::getInstance()
                .getParentContext()
                .getAllEntities()
                .empty());
    REQUIRE(ExtractionContext::getInstance()
                .getFollowsContext()
                .getAllEntities()
                .empty());
    REQUIRE(!ExtractionContext::getInstance().getUsingStatement().has_value());
    REQUIRE(
        !ExtractionContext::getInstance().getModifyingStatement().has_value());
}

TEST_CASE("ExtractionContext: Correctly pushes and pops Entities") {
    const string PROC_NAME = "TEST_PROC";
    Procedure procedure(PROC_NAME);
    REQUIRE(
        !ExtractionContext::getInstance().getCurrentProcedure().has_value());
    ExtractionContext::getInstance().setCurrentProcedure(&procedure);
    REQUIRE(ExtractionContext::getInstance().getCurrentProcedure().has_value());
    REQUIRE(ExtractionContext::getInstance()
                .getCurrentProcedure()
                .value()
                ->getName() == PROC_NAME);
}

TEST_CASE("ExtractionContext: Correctly sets and unsets statements") {
    const string PROC_NAME = "TEST_PROC";
    Statement callStatement(1, StatementType::CALL);
    callStatement.setProcName(PROC_NAME);
    REQUIRE(!ExtractionContext::getInstance().getUsingStatement().has_value());
    ExtractionContext::getInstance().setUsingStatement(&callStatement);
    REQUIRE(ExtractionContext::getInstance().getUsingStatement().has_value());
    REQUIRE(ExtractionContext::getInstance().getUsingStatement().value() ==
            &callStatement);
}