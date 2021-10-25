#include "../TestUtil.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

/**
 * TODO: Refactor tests once we fix validation for cond_expr
 * https://github.com/nus-cs3203/21s1-cp-spa-team-40/issues/148
 */

struct TestExtractWhileStatement {
    static PKB pkb;
    static ProcName PROC_NAME;
    static VarName VAR_NAME;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractWhileStatement::pkb = PKB();
ProcName TestExtractWhileStatement::PROC_NAME = "PROC";
VarName TestExtractWhileStatement::VAR_NAME = "VAR";

TEST_CASE("TestExtractWhileStatement: Correctly extracts a simple "
          "WhileStatement") {
    TestExtractWhileStatement::reset();

    Program program;
    Procedure procedure(TestExtractWhileStatement::PROC_NAME);
    Statement whileStatement(1, StatementType::WHILE);
    Statement thenStatement(2, StatementType::READ);
    Variable variable(TestExtractWhileStatement::VAR_NAME);

    addConditionalExpression(&whileStatement);

    whileStatement.setProcName(procedure.getName());
    whileStatement.addThenStmt(&thenStatement);
    thenStatement.setVariable(&variable);
    procedure.addToStmtLst(&whileStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractWhileStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractWhileStatement::pkb.getAllProcs().asVector().size() ==
            1);
    REQUIRE(TestExtractWhileStatement::pkb.getAllStmts().asVector().size() ==
            2);
    REQUIRE(TestExtractWhileStatement::pkb.getAllStmts(StatementType::WHILE)
                .asVector()
                .size() == 1);
    REQUIRE(TestExtractWhileStatement::pkb.getParentStmt(
                thenStatement.getIndex()) == whileStatement.getIndex());
    REQUIRE(TestExtractWhileStatement::pkb
                .getParentStarStmts(thenStatement.getIndex())
                .size() == 1);
    REQUIRE(TestExtractWhileStatement::pkb
                .getParentStarStmts(thenStatement.getIndex())
                .count(whileStatement.getIndex()));
}

TEST_CASE(
    "TestExtractWhileStatement: Correctly extracts a nested WhileStatement") {
    TestExtractWhileStatement::reset();

    Program program;
    Procedure procedure(TestExtractWhileStatement::PROC_NAME);
    Statement whileStatement(1, StatementType::WHILE);
    Statement thenWhileStatement(2, StatementType::WHILE);
    Statement thenWhileThenStatement(3, StatementType::READ);
    Variable variable(TestExtractWhileStatement::VAR_NAME);

    addConditionalExpression(&whileStatement);
    addConditionalExpression(&thenWhileStatement);

    whileStatement.setProcName(procedure.getName());
    whileStatement.addThenStmt(&thenWhileStatement);
    thenWhileStatement.addThenStmt(&thenWhileThenStatement);
    thenWhileThenStatement.setVariable(&variable);
    procedure.addToStmtLst(&whileStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractWhileStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractWhileStatement::pkb.getAllProcs().asVector().size() ==
            1);
    REQUIRE(TestExtractWhileStatement::pkb.getAllStmts().asVector().size() ==
            3);
    REQUIRE(TestExtractWhileStatement::pkb.getAllStmts(StatementType::WHILE)
                .asVector()
                .size() == 2);

    // Check that top level whileStatement is parent of thenWhileStatement
    REQUIRE(TestExtractWhileStatement::pkb.getParentStmt(
                thenWhileStatement.getIndex()) == whileStatement.getIndex());

    // Check that thenWhileStatement is parent of thenWhileThenStatement
    REQUIRE(TestExtractWhileStatement::pkb.getParentStmt(
                thenWhileThenStatement.getIndex()) ==
            thenWhileStatement.getIndex());

    // Check that thenWhileThenStatement has both thenWhileStatement and
    // whileStatement as transitive parents
    REQUIRE(TestExtractWhileStatement::pkb
                .getParentStarStmts(thenWhileThenStatement.getIndex())
                .size() == 2);
    REQUIRE(TestExtractWhileStatement::pkb
                .getParentStarStmts(thenWhileThenStatement.getIndex())
                .count(thenWhileStatement.getIndex()));
    REQUIRE(TestExtractWhileStatement::pkb
                .getParentStarStmts(thenWhileThenStatement.getIndex())
                .count(whileStatement.getIndex()));
}

TEST_CASE("TestExtractWhileStatement: Correctly extracts Follows relationship "
          "in THEN StatementList") {
    TestExtractWhileStatement::reset();

    Program program;
    Procedure procedure(TestExtractWhileStatement::PROC_NAME);
    Statement whileStatement(1, StatementType::WHILE);
    Statement thenStatement1(2, StatementType::READ);
    Statement thenStatement2(3, StatementType::READ);
    Statement thenStatement3(4, StatementType::READ);
    Variable variable(TestExtractWhileStatement::VAR_NAME);

    addConditionalExpression(&whileStatement);

    whileStatement.addThenStmt(&thenStatement1);
    whileStatement.addThenStmt(&thenStatement2);
    whileStatement.addThenStmt(&thenStatement3);
    thenStatement1.setVariable(&variable);
    thenStatement2.setVariable(&variable);
    thenStatement3.setVariable(&variable);
    procedure.addToStmtLst(&whileStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractWhileStatement::pkb);
    de.extract(&program);

    auto follow1 = TestExtractWhileStatement::pkb.getFollowingStmt(
        thenStatement1.getIndex());
    auto followStar1 = TestExtractWhileStatement::pkb.getFollowingStarStmts(
        thenStatement1.getIndex());
    auto follow2 = TestExtractWhileStatement::pkb.getFollowingStmt(
        thenStatement2.getIndex());
    auto followStar2 = TestExtractWhileStatement::pkb.getFollowingStarStmts(
        thenStatement2.getIndex());
    auto follow3 = TestExtractWhileStatement::pkb.getFollowingStmt(
        thenStatement3.getIndex());
    auto followStar3 = TestExtractWhileStatement::pkb.getFollowingStarStmts(
        thenStatement3.getIndex());

    REQUIRE(follow1 == thenStatement2.getIndex());
    REQUIRE(followStar1.size() == 2);
    REQUIRE(followStar1.count(thenStatement2.getIndex()));
    REQUIRE(followStar1.count(thenStatement3.getIndex()));

    REQUIRE(follow2 == thenStatement3.getIndex());
    REQUIRE(followStar2.size() == 1);
    REQUIRE(followStar2.count(thenStatement3.getIndex()));

    REQUIRE(follow3 == -1);
    REQUIRE(followStar3.empty());
}

TEST_CASE("TestExtractWhileStatement: Correctly extracts Next relationship.") {
    TestExtractWhileStatement::reset();

    Program program;
    Procedure procedure(TestExtractWhileStatement::PROC_NAME);
    Statement whileStatement(1, StatementType::WHILE);
    Statement thenStatement1(2, StatementType::READ);
    Statement thenStatement2(3, StatementType::READ);
    Statement thenStatement3(4, StatementType::READ);
    Statement afterWhileStatement(5, StatementType::READ);
    Variable variable(TestExtractWhileStatement::VAR_NAME);

    addConditionalExpression(&whileStatement);

    whileStatement.addThenStmt(&thenStatement1);
    whileStatement.addThenStmt(&thenStatement2);
    whileStatement.addThenStmt(&thenStatement3);

    thenStatement1.setVariable(&variable);
    thenStatement2.setVariable(&variable);
    thenStatement3.setVariable(&variable);
    afterWhileStatement.setVariable(&variable);

    procedure.addToStmtLst(&whileStatement);
    procedure.addToStmtLst(&afterWhileStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractWhileStatement::pkb);
    de.extract(&program);

    // Check Next(w, s1) where s1 is the first THEN statement and s2 is the
    // statement which follows immediately after the WHILE statement
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(whileStatement.getIndex())
            .size() == 2);
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(whileStatement.getIndex())
            .count(afterWhileStatement.getIndex()));
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(whileStatement.getIndex())
            .count(thenStatement1.getIndex()));

    // Check Next(s1, s2) where s1, s2 are consecutive THEN statements
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement1.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement1.getIndex())
            .count(thenStatement2.getIndex()));
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement2.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement2.getIndex())
            .count(thenStatement3.getIndex()));

    // Check Next(s, w) where s is the last THEN statement in while w
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement3.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement3.getIndex())
            .count(whileStatement.getIndex()));
}

TEST_CASE(
    "TestExtractWhileStatement: Correctly extracts Next relationship in a "
    "WhileStatement ending with an IfStatement.") {
    TestExtractWhileStatement::reset();

    Program program;
    Procedure procedure(TestExtractWhileStatement::PROC_NAME);
    Statement whileStatement(1, StatementType::WHILE);
    Statement thenStatement1(2, StatementType::READ);
    Statement thenStatement2(3, StatementType::READ);
    Statement thenIfStatement(4, StatementType::IF);
    Statement thenIfThenStatement1(5, StatementType::READ);
    Statement thenIfThenStatement2(6, StatementType::READ);
    Statement thenIfThenStatement3(7, StatementType::READ);
    Statement thenIfElseStatement1(8, StatementType::READ);
    Statement thenIfElseStatement2(9, StatementType::READ);
    Statement thenIfElseStatement3(10, StatementType::READ);
    Statement afterWhileStatement(11, StatementType::READ);

    Variable variable(TestExtractWhileStatement::VAR_NAME);

    addConditionalExpression(&whileStatement);
    addConditionalExpression(&thenIfStatement);

    whileStatement.addThenStmt(&thenStatement1);
    whileStatement.addThenStmt(&thenStatement2);
    whileStatement.addThenStmt(&thenIfStatement);

    thenIfStatement.addThenStmt(&thenIfThenStatement1);
    thenIfStatement.addThenStmt(&thenIfThenStatement2);
    thenIfStatement.addThenStmt(&thenIfThenStatement3);
    thenIfStatement.addElseStmt(&thenIfElseStatement1);
    thenIfStatement.addElseStmt(&thenIfElseStatement2);
    thenIfStatement.addElseStmt(&thenIfElseStatement3);

    thenStatement1.setVariable(&variable);
    thenStatement2.setVariable(&variable);
    thenIfThenStatement1.setVariable(&variable);
    thenIfThenStatement2.setVariable(&variable);
    thenIfThenStatement3.setVariable(&variable);
    thenIfElseStatement1.setVariable(&variable);
    thenIfElseStatement2.setVariable(&variable);
    thenIfElseStatement3.setVariable(&variable);
    afterWhileStatement.setVariable(&variable);

    procedure.addToStmtLst(&whileStatement);
    procedure.addToStmtLst(&afterWhileStatement);
    program.addToProcLst(&procedure);

    /**
     * THE ABOVE PROGRAM TRANSLATES TO:
     * ****************** START OF PROGRAM ******************
     * procedure PROC_NAME {
     *   while (...) {             (1)
     *      read x;                (2)
     *      read x;                (3)
     *      if (...) {             (4)
     *        read x;              (5)
     *        read x;              (6)
     *        read x;              (7)
     *      } else {
     *        read x;              (8)
     *        read x;              (9)
     *        read x;              (10)
     *      {
     *    }
     *    read x;                  (11)
     * }
     * ****************** END OF PROGRAM ******************
     */

    DesignExtractor de(&TestExtractWhileStatement::pkb);
    de.extract(&program);
    // Check Next(w, s1), Next(ifs, s2) where s1 is the first THEN
    // and s2 is the statement which follows immediately after while w
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(whileStatement.getIndex())
            .size() == 2);
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(whileStatement.getIndex())
            .count(thenStatement1.getIndex()));
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(whileStatement.getIndex())
            .count(afterWhileStatement.getIndex()));

    // Check Next(s1, s2) where s1, s2 are consecutive THEN statements
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement1.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement1.getIndex())
            .count(thenStatement2.getIndex()));
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement2.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractWhileStatement::pkb.getNextLines(thenStatement2.getIndex())
            .count(thenIfStatement.getIndex()));

    // Check that Next(s1, w), Next(s2, w) where s1, s2 are the last THEN/ELSE
    // statements belonging to the IfStatement respectively
    REQUIRE(TestExtractWhileStatement::pkb
                .getNextLines(thenIfThenStatement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractWhileStatement::pkb
                .getNextLines(thenIfThenStatement3.getIndex())
                .count(whileStatement.getIndex()));
    REQUIRE(TestExtractWhileStatement::pkb
                .getNextLines(thenIfElseStatement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractWhileStatement::pkb
                .getNextLines(thenIfElseStatement3.getIndex())
                .count(whileStatement.getIndex()));
}
