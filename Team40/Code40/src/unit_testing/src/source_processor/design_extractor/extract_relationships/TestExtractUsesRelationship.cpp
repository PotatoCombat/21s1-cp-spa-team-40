#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractUsesRelationship {
    static PKB pkb;
    static ProcName PROC_NAME_1;
    static ProcName PROC_NAME_2;
    static VarName VAR_NAME;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractUsesRelationship::pkb = PKB();
ProcName TestExtractUsesRelationship::PROC_NAME_1 = "PROC_1";
ProcName TestExtractUsesRelationship::PROC_NAME_2 = "PROC_2";
VarName TestExtractUsesRelationship::VAR_NAME = "VAR";

TEST_CASE("TestExtractUsesRelationship: Correct extracts a simple Uses(s, v)") {
    TestExtractUsesRelationship::reset();

    Program program;
    Procedure procedure(TestExtractUsesRelationship::PROC_NAME_1);
    Statement statement(1, StatementType::PRINT);
    Variable variable(TestExtractUsesRelationship::VAR_NAME);

    statement.setVariable(&variable);
    procedure.addToStmtLst(&statement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getIndex())
            .count(variable.getName()));
}

TEST_CASE("TestExtractUsesRelationship: Correct extracts Uses(p, v) where "
          "Uses(s, v)") {
    TestExtractUsesRelationship::reset();

    Program program;
    Procedure procedure(TestExtractUsesRelationship::PROC_NAME_1);
    Statement statement(1, StatementType::PRINT);
    Variable variable(TestExtractUsesRelationship::VAR_NAME);

    statement.setVariable(&variable);
    procedure.addToStmtLst(&statement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getIndex())
            .count(variable.getName()));
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure.getName())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure.getName())
            .count(variable.getName()));
}

TEST_CASE("TestExtractUsesRelationship: Correct extracts Uses(p1, v) where "
          "Uses(p2, v) and Calls(p1, p2)") {
    TestExtractUsesRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractUsesRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractUsesRelationship::PROC_NAME_2);
    Statement callStatement(1, StatementType::CALL);
    Statement printStatement(2, StatementType::PRINT);
    Variable variable(TestExtractUsesRelationship::VAR_NAME);

    callStatement.setProcName(procedure2.getName());
    printStatement.setVariable(&variable);
    procedure1.addToStmtLst(&callStatement);
    procedure2.addToStmtLst(&printStatement);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    // Check that Uses(p2, v)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure2.getName())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure2.getName())
            .count(variable.getName()));

    // Check that Calls(p1, p2)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getCalledProcs(procedure1.getName())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getCalledProcs(procedure1.getName())
            .count(procedure2.getName()));

    // Check that Uses(p1, v)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure1.getName())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure1.getName())
            .count(variable.getName()));
}
TEST_CASE("TestExtractUsesRelationship: Correct extracts a Uses(p1, v) where "
          "Uses(p2, v) and Calls(p1, p2)") {
    TestExtractUsesRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractUsesRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractUsesRelationship::PROC_NAME_2);
    Statement callStatement(1, StatementType::CALL);
    Statement printStatement(2, StatementType::PRINT);
    Variable variable(TestExtractUsesRelationship::VAR_NAME);

    callStatement.setProcName(procedure2.getName());
    printStatement.setVariable(&variable);
    procedure1.addToStmtLst(&callStatement);
    procedure2.addToStmtLst(&printStatement);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    // Check that Uses(p2, v)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure2.getName())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure2.getName())
            .count(variable.getName()));

    // Check that Calls(p1, p2)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getCalledProcs(procedure1.getName())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getCalledProcs(procedure1.getName())
            .count(procedure2.getName()));

    // Check that Uses(p1, v)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure1.getName())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure1.getName())
            .count(variable.getName()));
}