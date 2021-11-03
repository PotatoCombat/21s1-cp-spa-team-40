#include "catch.hpp"
#include "source_processor/design_extractor/ExtractionContext.h"

TEST_CASE("TestExtractionContext") {
    ExtractionContext::getInstance().reset();

    SECTION("Creates correctly") {
        ExtractionContext::getInstance().reset();
        REQUIRE(!ExtractionContext::getInstance()
                     .getCurrentProcedure()
                     .has_value());
        REQUIRE(ExtractionContext::getInstance().getParentStatements().empty());
        REQUIRE(
            ExtractionContext::getInstance().getPrecedingStatements().empty());
        REQUIRE(
            !ExtractionContext::getInstance().getUsingStatement().has_value());
        REQUIRE(!ExtractionContext::getInstance()
                     .getModifyingStatement()
                     .has_value());
    }

    SECTION("Correctly pushes and pops Entities") {
        ExtractionContext::getInstance().reset();
        const string PROC_NAME = "TEST_PROC";
        Procedure procedure(PROC_NAME);
        REQUIRE(!ExtractionContext::getInstance()
                     .getCurrentProcedure()
                     .has_value());
        ExtractionContext::getInstance().setCurrentProcedure(&procedure);
        REQUIRE(
            ExtractionContext::getInstance().getCurrentProcedure().has_value());
        REQUIRE(ExtractionContext::getInstance()
                    .getCurrentProcedure()
                    .value()
                    ->getId() == PROC_NAME);
    }

    SECTION("Correctly sets and unsets statements") {
        ExtractionContext::getInstance().reset();
        const string PROC_NAME = "TEST_PROC";
        Statement callStatement(1, StatementType::CALL);
        callStatement.setProcName(PROC_NAME);
        REQUIRE(
            !ExtractionContext::getInstance().getUsingStatement().has_value());
        ExtractionContext::getInstance().setUsingStatement(&callStatement);
        REQUIRE(
            ExtractionContext::getInstance().getUsingStatement().has_value());
        REQUIRE(ExtractionContext::getInstance().getUsingStatement().value() ==
                &callStatement);
    }

    SECTION("Correctly builds dependency tree for Procedures") {
        ExtractionContext::getInstance().reset();
        const ProcName proc1 = "PROC_1";
        const ProcName proc2 = "PROC_2";
        const ProcName proc3 = "PROC_3";
        const ProcName proc4 = "PROC_4";
        const ProcName proc5 = "PROC_5";

        ExtractionContext::getInstance().registerProcDependency(proc1, proc2);
        ExtractionContext::getInstance().registerProcDependency(proc1, proc5);
        ExtractionContext::getInstance().registerProcDependency(proc2, proc3);
        ExtractionContext::getInstance().registerProcDependency(proc2, proc4);
        ExtractionContext::getInstance().registerProcDependency(proc2, proc5);
        ExtractionContext::getInstance().registerProcDependency(proc4, proc5);

        auto deps1 =
            ExtractionContext::getInstance().getProcDependencies(proc1);
        REQUIRE(deps1.count(proc2));
        REQUIRE(!deps1.count(proc3));
        REQUIRE(!deps1.count(proc4));
        REQUIRE(deps1.count(proc5));

        auto deps2 =
            ExtractionContext::getInstance().getProcDependencies(proc2);
        REQUIRE(!deps2.count(proc1));
        REQUIRE(deps2.count(proc3));
        REQUIRE(deps2.count(proc4));
        REQUIRE(deps2.count(proc5));

        auto deps3 =
            ExtractionContext::getInstance().getProcDependencies(proc3);
        REQUIRE(deps3.empty());

        auto deps4 =
            ExtractionContext::getInstance().getProcDependencies(proc4);
        REQUIRE(!deps4.count(proc1));
        REQUIRE(!deps4.count(proc2));
        REQUIRE(!deps4.count(proc3));
        REQUIRE(deps4.count(proc5));

        auto deps5 =
            ExtractionContext::getInstance().getProcDependencies(proc5);
        REQUIRE(deps5.empty());
    }

    SECTION("Permits one-to-many dependencies") {
        ExtractionContext::getInstance().reset();
        const ProcName proc1 = "PROC_1";
        const ProcName proc2 = "PROC_2";
        const ProcName proc3 = "PROC_3";

        REQUIRE_NOTHROW(ExtractionContext::getInstance().registerProcDependency(
            proc1, proc2));
        REQUIRE_NOTHROW(ExtractionContext::getInstance().registerProcDependency(
            proc1, proc3));
    }

    SECTION("Permits many-to-one dependencies") {
        ExtractionContext::getInstance().reset();
        const ProcName proc1 = "PROC_1";
        const ProcName proc2 = "PROC_2";
        const ProcName proc3 = "PROC_3";

        REQUIRE_NOTHROW(ExtractionContext::getInstance().registerProcDependency(
            proc1, proc3));
        REQUIRE_NOTHROW(ExtractionContext::getInstance().registerProcDependency(
            proc2, proc3));
    }

    SECTION("Correctly detects recursion") {
        ExtractionContext::getInstance().reset();
        const ProcName proc1 = "PROC_1";

        REQUIRE_THROWS(ExtractionContext::getInstance().registerProcDependency(
            proc1, proc1));
    }

    SECTION("Correctly detects cyclical dependency") {
        ExtractionContext::getInstance().reset();
        const ProcName proc1 = "PROC_1";
        const ProcName proc2 = "PROC_2";
        const ProcName proc3 = "PROC_3";

        REQUIRE_NOTHROW(ExtractionContext::getInstance().registerProcDependency(
            proc1, proc2));
        REQUIRE_NOTHROW(ExtractionContext::getInstance().registerProcDependency(
            proc2, proc3));
        REQUIRE_THROWS(ExtractionContext::getInstance().registerProcDependency(
            proc3, proc1));
    }
}
