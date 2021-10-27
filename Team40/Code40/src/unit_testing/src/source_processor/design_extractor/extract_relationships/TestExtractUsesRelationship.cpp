#include "../TestUtil.h"
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
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getId())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getId())
            .count(variable.getId()));
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
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getId())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByStmt(statement.getId())
            .count(variable.getId()));
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure.getId())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure.getId())
            .count(variable.getId()));
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

    callStatement.setProcName(procedure2.getId());
    printStatement.setVariable(&variable);
    procedure1.addToStmtLst(&callStatement);
    procedure2.addToStmtLst(&printStatement);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    // Check that Uses(p2, v)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure2.getId())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure2.getId())
            .count(variable.getId()));

    // Check that Calls(p1, p2)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getCalledProcs(procedure1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getCalledProcs(procedure1.getId())
            .count(procedure2.getId()));

    // Check that Uses(p1, v)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure1.getId())
            .count(variable.getId()));
}

TEST_CASE("TestExtractUsesRelationship: Correct extracts Uses(s1, v) where "
          "Uses(s2, v) and Parent(s1, s2)") {
    TestExtractUsesRelationship::reset();

    Program program;
    Procedure procedure(TestExtractUsesRelationship::PROC_NAME_1);
    Statement whileStatement(1, StatementType::WHILE);
    Statement printStatement(2, StatementType::PRINT);
    Variable variable(TestExtractUsesRelationship::VAR_NAME);

    TestUtil::addConditionalExpression(&whileStatement);

    whileStatement.addThenStmt(&printStatement);
    printStatement.setVariable(&variable);
    procedure.addToStmtLst(&whileStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    // Check that Uses(s2, v)
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(printStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(printStatement.getId())
                .count(variable.getId()));

    // Check that Parent(s1, s2)
    REQUIRE(TestExtractUsesRelationship::pkb.getParentStmt(
                printStatement.getId()) == whileStatement.getId());

    // Check that Uses(p1, v)
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(whileStatement.getId())
                .count(variable.getId()));
}

TEST_CASE("TestExtractUsesRelationship: Correct extracts transitive Uses for a "
          "call statement in a while loop") {
    TestExtractUsesRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractUsesRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractUsesRelationship::PROC_NAME_2);
    Statement whileStatement(1, StatementType::WHILE);
    Statement callStatement(2, StatementType::CALL);
    Statement printStatement(2, StatementType::PRINT);
    Variable variable(TestExtractUsesRelationship::VAR_NAME);

    TestUtil::addConditionalExpression(&whileStatement);

    whileStatement.addThenStmt(&callStatement);
    callStatement.setProcName(procedure2.getId());
    printStatement.setVariable(&variable);
    procedure1.addToStmtLst(&whileStatement);
    procedure2.addToStmtLst(&printStatement);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    /**
     * THE ABOVE PROGRAM TRANSLATES TO:
     * ****************** START OF PROGRAM ******************
     * procedure PROC_1 {
     *   while (...) {             (1)
     *      call PROC_2;           (2)
     *   }
     * }
     * procedure PROC_1 {
     *   print x;                  (3)
     * }
     * ****************** END OF PROGRAM ******************
     */

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    // Check that Uses(printStatement, variable)
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(printStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(printStatement.getId())
                .count(variable.getId()));

    // Check that Calls(procedure1, procedure2)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getCalledProcs(procedure1.getId())
            .count(procedure2.getId()));
    // Check that Parent(whileStatement, callStatement)
    REQUIRE(TestExtractUsesRelationship::pkb.getParentStmt(
                callStatement.getId()) == whileStatement.getId());

    // Check that Uses(whileStatement, variable)
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(whileStatement.getId())
                .count(variable.getId()));

    // Check that Uses(procedure1, variable)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure1.getId())
            .count(variable.getId()));
}

TEST_CASE("TestExtractUsesRelationship: Correct extracts transitive Uses for a "
          "call statement in a if statement") {
    TestExtractUsesRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractUsesRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractUsesRelationship::PROC_NAME_2);
    Statement ifStatement(1, StatementType::IF);
    Statement callStatement(2, StatementType::CALL);
    Statement printStatement(2, StatementType::PRINT);
    Variable variable(TestExtractUsesRelationship::VAR_NAME);

    TestUtil::addConditionalExpression(&ifStatement);

    ifStatement.addThenStmt(&callStatement);
    ifStatement.addElseStmt(&callStatement);
    callStatement.setProcName(procedure2.getId());
    printStatement.setVariable(&variable);
    procedure1.addToStmtLst(&ifStatement);
    procedure2.addToStmtLst(&printStatement);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    /**
     * THE ABOVE PROGRAM TRANSLATES TO:
     * ****************** START OF PROGRAM ******************
     * procedure PROC_1 {
     *   if (...) {             (1)
     *      call PROC_2;        (2)
     *   } else {
     *      call PROC_2;        (3)
     *   }
     * }
     * procedure PROC_1 {
     *   print x;               (4)
     * }
     * ****************** END OF PROGRAM ******************
     */

    DesignExtractor de(&TestExtractUsesRelationship::pkb);
    de.extract(&program);

    // Check that Uses(printStatement, variable)
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(printStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(printStatement.getId())
                .count(variable.getId()));

    // Check that Calls(procedure1, procedure2)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getCalledProcs(procedure1.getId())
            .count(procedure2.getId()));
    // Check that Parent(whileStatement, callStatement)
    REQUIRE(TestExtractUsesRelationship::pkb.getParentStmt(
                callStatement.getId()) == ifStatement.getId());

    // Check that Uses(whileStatement, variable)
    REQUIRE(TestExtractUsesRelationship::pkb
                .getVarsUsedByStmt(ifStatement.getId())
                .count(variable.getId()));

    // Check that Uses(procedure1, variable)
    REQUIRE(
        TestExtractUsesRelationship::pkb.getVarsUsedByProc(procedure1.getId())
            .count(variable.getId()));
}