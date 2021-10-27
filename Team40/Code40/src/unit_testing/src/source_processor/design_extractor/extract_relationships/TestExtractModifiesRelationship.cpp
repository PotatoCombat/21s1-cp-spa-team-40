#include "../TestUtil.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractModifiesRelationship {
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

PKB TestExtractModifiesRelationship::pkb = PKB();
ProcName TestExtractModifiesRelationship::PROC_NAME_1 = "PROC_1";
ProcName TestExtractModifiesRelationship::PROC_NAME_2 = "PROC_2";
VarName TestExtractModifiesRelationship::VAR_NAME = "VAR";

TEST_CASE("TestExtractModifiesRelationship: Correct extracts a simple "
          "Modifies(s, v)") {
    TestExtractModifiesRelationship::reset();

    Program program;
    Procedure procedure(TestExtractModifiesRelationship::PROC_NAME_1);
    Statement statement(1, StatementType::READ);
    Variable variable(TestExtractModifiesRelationship::VAR_NAME);

    statement.setVariable(&variable);
    procedure.addToStmtLst(&statement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractModifiesRelationship::pkb);
    de.extract(&program);

    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(statement.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(statement.getId())
                .count(variable.getId()));
}

TEST_CASE(
    "TestExtractModifiesRelationship: Correct extracts Modifies(p, v) where "
    "Modifies(s, v)") {
    TestExtractModifiesRelationship::reset();

    Program program;
    Procedure procedure(TestExtractModifiesRelationship::PROC_NAME_1);
    Statement statement(1, StatementType::READ);
    Variable variable(TestExtractModifiesRelationship::VAR_NAME);

    statement.setVariable(&variable);
    procedure.addToStmtLst(&statement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractModifiesRelationship::pkb);
    de.extract(&program);

    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(statement.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(statement.getId())
                .count(variable.getId()));
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure.getId())
                .count(variable.getId()));
}

TEST_CASE(
    "TestExtractModifiesRelationship: Correct extracts Modifies(p1, v) where "
    "Modifies(p2, v) and Calls(p1, p2)") {
    TestExtractModifiesRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractModifiesRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractModifiesRelationship::PROC_NAME_2);
    Statement callStatement(1, StatementType::CALL);
    Statement readStatement(2, StatementType::READ);
    Variable variable(TestExtractModifiesRelationship::VAR_NAME);

    callStatement.setProcName(procedure2.getId());
    readStatement.setVariable(&variable);
    procedure1.addToStmtLst(&callStatement);
    procedure2.addToStmtLst(&readStatement);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    DesignExtractor de(&TestExtractModifiesRelationship::pkb);
    de.extract(&program);

    // Check that Modifies(p2, v)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure2.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure2.getId())
                .count(variable.getId()));

    // Check that Calls(p1, p2)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getCalledProcs(procedure1.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getCalledProcs(procedure1.getId())
                .count(procedure2.getId()));

    // Check that Modifies(p1, v)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getId())
                .count(variable.getId()));
}

TEST_CASE(
    "TestExtractModifiesRelationship: Correct extracts Modifies(s1, v) where "
    "Modifies(s2, v) and Parent(s1, s2)") {
    TestExtractModifiesRelationship::reset();

    Program program;
    Procedure procedure(TestExtractModifiesRelationship::PROC_NAME_1);
    Statement whileStatement(1, StatementType::WHILE);
    Statement readStatement(2, StatementType::READ);
    Variable variable(TestExtractModifiesRelationship::VAR_NAME);

    TestUtil::addConditionalExpression(&whileStatement);

    whileStatement.addThenStmt(&readStatement);
    readStatement.setVariable(&variable);
    procedure.addToStmtLst(&whileStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractModifiesRelationship::pkb);
    de.extract(&program);

    // Check that Modifies(s2, v)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getId())
                .count(variable.getId()));

    // Check that Parent(s1, s2)
    REQUIRE(TestExtractModifiesRelationship::pkb.getParentStmt(
                readStatement.getId()) == whileStatement.getId());

    // Check that Modifies(p1, v)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(whileStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(whileStatement.getId())
                .count(variable.getId()));
}

TEST_CASE("TestExtractModifiesRelationship: Correct extracts transitive "
          "Modifies for a call statement in a while loop") {
    TestExtractModifiesRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractModifiesRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractModifiesRelationship::PROC_NAME_2);
    Statement whileStatement(1, StatementType::WHILE);
    Statement callStatement(2, StatementType::CALL);
    Statement readStatement(2, StatementType::READ);
    Variable variable(TestExtractModifiesRelationship::VAR_NAME);

    TestUtil::addConditionalExpression(&whileStatement);

    whileStatement.addThenStmt(&callStatement);
    callStatement.setProcName(procedure2.getId());
    readStatement.setVariable(&variable);
    procedure1.addToStmtLst(&whileStatement);
    procedure2.addToStmtLst(&readStatement);
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
     *   read x;                  (3)
     * }
     * ****************** END OF PROGRAM ******************
     */

    DesignExtractor de(&TestExtractModifiesRelationship::pkb);
    de.extract(&program);

    // Check that Modifies(readStatement, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getId())
                .count(variable.getId()));

    // Check that Calls(procedure1, procedure2)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getCalledProcs(procedure1.getId())
                .count(procedure2.getId()));
    // Check that Parent(whileStatement, callStatement)
    REQUIRE(TestExtractModifiesRelationship::pkb.getParentStmt(
                callStatement.getId()) == whileStatement.getId());

    // Check that Modifies(whileStatement, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(whileStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(whileStatement.getId())
                .count(variable.getId()));

    // Check that Modifies(procedure1, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getId())
                .count(variable.getId()));
}

TEST_CASE("TestExtractModifiesRelationship: Correct extracts transitive "
          "Modifies for a call statement in a if statement") {
    TestExtractModifiesRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractModifiesRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractModifiesRelationship::PROC_NAME_2);
    Statement ifStatement(1, StatementType::IF);
    Statement callStatement(2, StatementType::CALL);
    Statement readStatement(2, StatementType::READ);
    Variable variable(TestExtractModifiesRelationship::VAR_NAME);

    TestUtil::addConditionalExpression(&ifStatement);

    ifStatement.addThenStmt(&callStatement);
    ifStatement.addElseStmt(&callStatement);
    callStatement.setProcName(procedure2.getId());
    readStatement.setVariable(&variable);
    procedure1.addToStmtLst(&ifStatement);
    procedure2.addToStmtLst(&readStatement);
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
     *   read x;               (4)
     * }
     * ****************** END OF PROGRAM ******************
     */

    DesignExtractor de(&TestExtractModifiesRelationship::pkb);
    de.extract(&program);

    // Check that Modifies(readStatement, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getId())
                .count(variable.getId()));

    // Check that Calls(procedure1, procedure2)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getCalledProcs(procedure1.getId())
                .count(procedure2.getId()));
    // Check that Parent(whileStatement, callStatement)
    REQUIRE(TestExtractModifiesRelationship::pkb.getParentStmt(
                callStatement.getId()) == ifStatement.getId());

    // Check that Modifies(whileStatement, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(ifStatement.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(ifStatement.getId())
                .count(variable.getId()));

    // Check that Modifies(procedure1, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getId())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getId())
                .count(variable.getId()));
}
