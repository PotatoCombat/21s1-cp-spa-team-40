#include "catch.hpp"
#include "common/model/CallStatement.h"
#include "source_processor/design_extractor/ExtractionContext.h"

TEST_CASE("ExtractionContext: Creates correctly") {
    REQUIRE(ExtractionContext::getInstance()
                .getProcedureContext()
                .getAllEntities()
                .empty());
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
    REQUIRE(ExtractionContext::getInstance()
                .getProcedureContext()
                .getAllEntities()
                .empty());
    ExtractionContext::getInstance().getProcedureContext().push(&procedure);
    REQUIRE(ExtractionContext::getInstance()
                .getProcedureContext()
                .getAllEntities()
                .size() == 1);
    REQUIRE(ExtractionContext::getInstance()
                .getProcedureContext()
                .getAllEntities()
                .front()
                ->getName() == PROC_NAME);
}

TEST_CASE("ExtractionContext: Correctly sets and unsets statements") {
    const string PROC_NAME = "TEST_PROC";
    Procedure procedure(PROC_NAME);
    CallStatement callStatement(1, procedure);
    REQUIRE(!ExtractionContext::getInstance().getUsingStatement().has_value());
    ExtractionContext::getInstance().setUsingStatement(&callStatement);
    REQUIRE(ExtractionContext::getInstance().getUsingStatement().has_value());
    REQUIRE(ExtractionContext::getInstance().getUsingStatement().value() ==
            &callStatement);
}