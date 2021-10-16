#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractUsesRelationship {
    static PKB pkb;
    static ProcName PROC_NAME;
    static VarName VAR_NAME;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractUsesRelationship::pkb = PKB();
ProcName TestExtractUsesRelationship::PROC_NAME = "PROC";
VarName TestExtractUsesRelationship::VAR_NAME = "VAR";

TEST_CASE("TestExtractUsesRelationship: Correct extracts a simple Uses(s, v)") {
    TestExtractUsesRelationship::reset();

    Program program;
    Procedure procedure(TestExtractUsesRelationship::PROC_NAME);
    Statement statement(1, StatementType::PRINT);
    Variable variable(TestExtractUsesRelationship::VAR_NAME);

    statement.setVariable(&variable);
    procedure.addToStmtLst(&statement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getIndex())
            .count(variable.getName()));
}