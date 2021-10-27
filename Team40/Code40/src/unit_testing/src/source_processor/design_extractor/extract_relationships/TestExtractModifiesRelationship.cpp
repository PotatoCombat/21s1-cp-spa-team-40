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
                .getVarsModifiedByStmt(statement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(statement.getIndex())
                .count(variable.getName()));
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
                .getVarsModifiedByStmt(statement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(statement.getIndex())
                .count(variable.getName()));
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure.getName())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure.getName())
                .count(variable.getName()));
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

    callStatement.setProcName(procedure2.getName());
    readStatement.setVariable(&variable);
    procedure1.addToStmtLst(&callStatement);
    procedure2.addToStmtLst(&readStatement);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    DesignExtractor de(&TestExtractModifiesRelationship::pkb);
    de.extract(&program);

    // Check that Modifies(p2, v)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure2.getName())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure2.getName())
                .count(variable.getName()));

    // Check that Calls(p1, p2)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getCalledProcs(procedure1.getName())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getCalledProcs(procedure1.getName())
                .count(procedure2.getName()));

    // Check that Modifies(p1, v)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getName())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getName())
                .count(variable.getName()));
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
                .getVarsModifiedByStmt(readStatement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getIndex())
                .count(variable.getName()));

    // Check that Parent(s1, s2)
    REQUIRE(TestExtractModifiesRelationship::pkb.getParentStmt(
                readStatement.getIndex()) == whileStatement.getIndex());

    // Check that Modifies(p1, v)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(whileStatement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(whileStatement.getIndex())
                .count(variable.getName()));
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
    callStatement.setProcName(procedure2.getName());
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
                .getVarsModifiedByStmt(readStatement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getIndex())
                .count(variable.getName()));

    // Check that Calls(procedure1, procedure2)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getCalledProcs(procedure1.getName())
                .count(procedure2.getName()));
    // Check that Parent(whileStatement, callStatement)
    REQUIRE(TestExtractModifiesRelationship::pkb.getParentStmt(
                callStatement.getIndex()) == whileStatement.getIndex());

    // Check that Modifies(whileStatement, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(whileStatement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(whileStatement.getIndex())
                .count(variable.getName()));

    // Check that Modifies(procedure1, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getName())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getName())
                .count(variable.getName()));
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
    callStatement.setProcName(procedure2.getName());
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
                .getVarsModifiedByStmt(readStatement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(readStatement.getIndex())
                .count(variable.getName()));

    // Check that Calls(procedure1, procedure2)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getCalledProcs(procedure1.getName())
                .count(procedure2.getName()));
    // Check that Parent(whileStatement, callStatement)
    REQUIRE(TestExtractModifiesRelationship::pkb.getParentStmt(
                callStatement.getIndex()) == ifStatement.getIndex());

    // Check that Modifies(whileStatement, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(ifStatement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByStmt(ifStatement.getIndex())
                .count(variable.getName()));

    // Check that Modifies(procedure1, variable)
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getName())
                .size() == 1);
    REQUIRE(TestExtractModifiesRelationship::pkb
                .getVarsModifiedByProc(procedure1.getName())
                .count(variable.getName()));
}
