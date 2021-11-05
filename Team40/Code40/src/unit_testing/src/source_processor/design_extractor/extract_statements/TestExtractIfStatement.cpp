#include "../TestUtil.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractIfStatement {
    static PKB pkb;
    static ProcName PROC_NAME;
    static VarName VAR_NAME;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractIfStatement::pkb = PKB();
ProcName TestExtractIfStatement::PROC_NAME = "PROC";
VarName TestExtractIfStatement::VAR_NAME = "VAR";

TEST_CASE("TestExtractIfStatement: Correctly extracts a simple IfStatement") {
    TestExtractIfStatement::reset();

    Program program;
    Procedure procedure(TestExtractIfStatement::PROC_NAME);
    Statement ifStatement(1, StatementType::IF);
    Statement thenStatement(2, StatementType::READ);
    Statement elseStatement(3, StatementType::READ);
    Variable variable(TestExtractIfStatement::VAR_NAME);

    TestUtil::addConditionalExpression(&ifStatement);

    ifStatement.setProcName(procedure.getId());
    ifStatement.addThenStmt(&thenStatement);
    ifStatement.addElseStmt(&elseStatement);
    thenStatement.setVariable(&variable);
    elseStatement.setVariable(&variable);
    procedure.addToStmtLst(&ifStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractIfStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractIfStatement::pkb.getAllProcs().asVector().size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getAllStmts().asVector().size() == 3);
    REQUIRE(TestExtractIfStatement::pkb.getAllStmts(StatementType::IF)
                .asVector()
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                thenStatement.getId()) == ifStatement.getId());
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                elseStatement.getId()) == ifStatement.getId());
    REQUIRE(
        TestExtractIfStatement::pkb.getParentStarStmts(thenStatement.getId())
            .size() == 1);
    REQUIRE(
        TestExtractIfStatement::pkb.getParentStarStmts(thenStatement.getId())
            .count(ifStatement.getId()));
    REQUIRE(
        TestExtractIfStatement::pkb.getParentStarStmts(elseStatement.getId())
            .size() == 1);
    REQUIRE(
        TestExtractIfStatement::pkb.getParentStarStmts(elseStatement.getId())
            .count(ifStatement.getId()));
}

TEST_CASE("TestExtractIfStatement: Correctly extracts a nested IfStatement") {
    TestExtractIfStatement::reset();

    Program program;
    Procedure procedure(TestExtractIfStatement::PROC_NAME);
    Statement ifStatement(1, StatementType::IF);
    Statement thenIfStatement(2, StatementType::IF);
    Statement thenIfThenStatement(3, StatementType::READ);
    Statement thenIfElseStatement(4, StatementType::READ);
    Statement elseStatement(5, StatementType::READ);
    Variable variable(TestExtractIfStatement::VAR_NAME);

    TestUtil::addConditionalExpression(&ifStatement);
    TestUtil::addConditionalExpression(&thenIfStatement);

    ifStatement.setProcName(procedure.getId());
    ifStatement.addThenStmt(&thenIfStatement);
    ifStatement.addElseStmt(&elseStatement);
    elseStatement.setVariable(&variable);
    thenIfStatement.addThenStmt(&thenIfThenStatement);
    thenIfStatement.addElseStmt(&thenIfElseStatement);
    thenIfThenStatement.setVariable(&variable);
    thenIfElseStatement.setVariable(&variable);
    procedure.addToStmtLst(&ifStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractIfStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractIfStatement::pkb.getAllProcs().asVector().size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getAllStmts().asVector().size() == 5);
    REQUIRE(TestExtractIfStatement::pkb.getAllStmts(StatementType::IF)
                .asVector()
                .size() == 2);

    // Check that top level ifStatement is parent of thenIfStatement &
    // elseStatement
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                thenIfStatement.getId()) == ifStatement.getId());
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                elseStatement.getId()) == ifStatement.getId());

    // Check that thenIfStatement is parent of thenIfThenStatement &
    // thenIfElseStatement
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                thenIfThenStatement.getId()) == thenIfStatement.getId());
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                thenIfElseStatement.getId()) == thenIfStatement.getId());

    // Check that thenIfThenStatement & thenIfElseStatement have 2
    // transitive parents each
    REQUIRE(TestExtractIfStatement::pkb
                .getParentStarStmts(thenIfThenStatement.getId())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb
                .getParentStarStmts(thenIfElseStatement.getId())
                .size() == 2);
}

TEST_CASE("TestExtractIfStatement: Correctly extracts Follows relationship "
          "in THEN and ELSE StatementList") {
    TestExtractIfStatement::reset();

    Program program;
    Procedure procedure(TestExtractIfStatement::PROC_NAME);
    Statement ifStatement(1, StatementType::IF);
    Statement thenStatement1(2, StatementType::READ);
    Statement thenStatement2(3, StatementType::READ);
    Statement thenStatement3(4, StatementType::READ);
    Statement elseStatement1(5, StatementType::READ);
    Statement elseStatement2(6, StatementType::READ);
    Statement elseStatement3(7, StatementType::READ);
    Variable variable(TestExtractIfStatement::VAR_NAME);

    TestUtil::addConditionalExpression(&ifStatement);

    ifStatement.addThenStmt(&thenStatement1);
    ifStatement.addThenStmt(&thenStatement2);
    ifStatement.addThenStmt(&thenStatement3);
    ifStatement.addElseStmt(&elseStatement1);
    ifStatement.addElseStmt(&elseStatement2);
    ifStatement.addElseStmt(&elseStatement3);

    thenStatement1.setVariable(&variable);
    thenStatement2.setVariable(&variable);
    thenStatement3.setVariable(&variable);
    elseStatement1.setVariable(&variable);
    elseStatement2.setVariable(&variable);
    elseStatement3.setVariable(&variable);

    procedure.addToStmtLst(&ifStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractIfStatement::pkb);
    de.extract(&program);

    auto thenFollow1 =
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement1.getId());
    auto thenFollowStar1 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        thenStatement1.getId());
    auto thenFollow2 =
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement2.getId());
    auto thenFollowStar2 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        thenStatement2.getId());
    auto thenFollow3 =
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement3.getId());
    auto thenFollowStar3 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        thenStatement3.getId());
    auto elseFollow1 =
        TestExtractIfStatement::pkb.getFollowingStmt(elseStatement1.getId());
    auto elseFollowStar1 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        elseStatement1.getId());
    auto elseFollow2 =
        TestExtractIfStatement::pkb.getFollowingStmt(elseStatement2.getId());
    auto elseFollowStar2 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        elseStatement2.getId());
    auto elseFollow3 =
        TestExtractIfStatement::pkb.getFollowingStmt(elseStatement3.getId());
    auto elseFollowStar3 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        elseStatement3.getId());

    REQUIRE(thenFollow1 == thenStatement2.getId());
    REQUIRE(thenFollowStar1.size() == 2);
    REQUIRE(thenFollowStar1.count(thenStatement2.getId()));
    REQUIRE(thenFollowStar1.count(thenStatement3.getId()));

    REQUIRE(thenFollow2 == thenStatement3.getId());
    REQUIRE(thenFollowStar2.size() == 1);
    REQUIRE(thenFollowStar2.count(thenStatement3.getId()));

    REQUIRE(thenFollow3 == -1);
    REQUIRE(thenFollowStar3.empty());

    REQUIRE(elseFollow1 == elseStatement2.getId());

    REQUIRE(elseFollowStar1.size() == 2);
    REQUIRE(elseFollowStar1.count(elseStatement2.getId()));
    REQUIRE(elseFollowStar1.count(elseStatement3.getId()));

    REQUIRE(elseFollow2 == elseStatement3.getId());
    REQUIRE(elseFollowStar2.size() == 1);
    REQUIRE(elseFollowStar2.count(elseStatement3.getId()));

    REQUIRE(elseFollow3 == -1);
    REQUIRE(elseFollowStar3.empty());
}

TEST_CASE("TestExtractIfStatement: Correctly extracts Next relationship.") {
    TestExtractIfStatement::reset();

    Program program;
    Procedure procedure(TestExtractIfStatement::PROC_NAME);
    Statement ifStatement(1, StatementType::IF);
    Statement thenStatement1(2, StatementType::READ);
    Statement thenStatement2(3, StatementType::READ);
    Statement thenStatement3(4, StatementType::READ);
    Statement elseStatement1(5, StatementType::READ);
    Statement elseStatement2(6, StatementType::READ);
    Statement elseStatement3(7, StatementType::READ);
    Statement afterIfStatement(8, StatementType::READ);
    Variable variable(TestExtractIfStatement::VAR_NAME);

    TestUtil::addConditionalExpression(&ifStatement);

    ifStatement.addThenStmt(&thenStatement1);
    ifStatement.addThenStmt(&thenStatement2);
    ifStatement.addThenStmt(&thenStatement3);
    ifStatement.addElseStmt(&elseStatement1);
    ifStatement.addElseStmt(&elseStatement2);
    ifStatement.addElseStmt(&elseStatement3);

    thenStatement1.setVariable(&variable);
    thenStatement2.setVariable(&variable);
    thenStatement3.setVariable(&variable);
    elseStatement1.setVariable(&variable);
    elseStatement2.setVariable(&variable);
    elseStatement3.setVariable(&variable);
    afterIfStatement.setVariable(&variable);

    procedure.addToStmtLst(&ifStatement);
    procedure.addToStmtLst(&afterIfStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractIfStatement::pkb);
    de.extract(&program);

    // Check Next(ifs, s1), Next(ifs, s2) where s1, s2 are the first THEN
    // and ELSE statements respectively
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getId())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getId())
                .count(thenStatement1.getId()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getId())
                .count(elseStatement1.getId()));

    // Check Next(s1, s2) where s1, s2 are consecutive THEN/ELSE statements
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement1.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement1.getId())
                .count(thenStatement2.getId()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement2.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement2.getId())
                .count(thenStatement3.getId()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement1.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement1.getId())
                .count(elseStatement2.getId()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement2.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement2.getId())
                .count(elseStatement3.getId()));

    // Check Next(s1, s2) where s1 is the last THEN/ELSE statement and s2 is
    // the statement which follows the IF statement
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement3.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement3.getId())
                .count(afterIfStatement.getId()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement3.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement3.getId())
                .count(afterIfStatement.getId()));
}

TEST_CASE("TestExtractIfStatement: Correctly extracts Next relationship in a "
          "nested IfStatement.") {
    TestExtractIfStatement::reset();

    Program program;
    Procedure procedure(TestExtractIfStatement::PROC_NAME);
    Statement ifStatement(1, StatementType::IF);
    Statement thenIfStatement(2, StatementType::IF);
    Statement thenIfThenStatement1(3, StatementType::READ);
    Statement thenIfThenStatement2(4, StatementType::READ);
    Statement thenIfThenStatement3(5, StatementType::READ);
    Statement thenIfElseStatement1(6, StatementType::READ);
    Statement thenIfElseStatement2(7, StatementType::READ);
    Statement thenIfElseStatement3(8, StatementType::READ);
    Statement elseIfStatement(9, StatementType::IF);
    Statement elseIfThenStatement1(10, StatementType::READ);
    Statement elseIfThenStatement2(11, StatementType::READ);
    Statement elseIfThenStatement3(12, StatementType::READ);
    Statement elseIfElseStatement1(13, StatementType::READ);
    Statement elseIfElseStatement2(14, StatementType::READ);
    Statement elseIfElseStatement3(15, StatementType::READ);
    Statement afterIfStatement(16, StatementType::READ);
    Variable variable(TestExtractIfStatement::VAR_NAME);

    TestUtil::addConditionalExpression(&ifStatement);
    TestUtil::addConditionalExpression(&thenIfStatement);
    TestUtil::addConditionalExpression(&elseIfStatement);

    ifStatement.addThenStmt(&thenIfStatement);
    ifStatement.addElseStmt(&elseIfStatement);
    thenIfStatement.addThenStmt(&thenIfThenStatement1);
    thenIfStatement.addThenStmt(&thenIfThenStatement2);
    thenIfStatement.addThenStmt(&thenIfThenStatement3);
    thenIfStatement.addElseStmt(&thenIfElseStatement1);
    thenIfStatement.addElseStmt(&thenIfElseStatement2);
    thenIfStatement.addElseStmt(&thenIfElseStatement3);
    elseIfStatement.addThenStmt(&elseIfThenStatement1);
    elseIfStatement.addThenStmt(&elseIfThenStatement2);
    elseIfStatement.addThenStmt(&elseIfThenStatement3);
    elseIfStatement.addElseStmt(&elseIfElseStatement1);
    elseIfStatement.addElseStmt(&elseIfElseStatement2);
    elseIfStatement.addElseStmt(&elseIfElseStatement3);

    elseIfThenStatement1.setVariable(&variable);
    elseIfThenStatement2.setVariable(&variable);
    elseIfThenStatement3.setVariable(&variable);
    elseIfElseStatement1.setVariable(&variable);
    elseIfElseStatement2.setVariable(&variable);
    elseIfElseStatement3.setVariable(&variable);
    thenIfThenStatement1.setVariable(&variable);
    thenIfThenStatement2.setVariable(&variable);
    thenIfThenStatement3.setVariable(&variable);
    thenIfElseStatement1.setVariable(&variable);
    thenIfElseStatement2.setVariable(&variable);
    thenIfElseStatement3.setVariable(&variable);
    afterIfStatement.setVariable(&variable);

    procedure.addToStmtLst(&ifStatement);
    procedure.addToStmtLst(&afterIfStatement);
    program.addToProcLst(&procedure);

    /**
     * THE ABOVE PROGRAM TRANSLATES TO:
     * ****************** START OF PROGRAM ******************
     * procedure PROC_NAME {
     *   if (...) {             (1)
     *      if (...) {          (2)
     *        read x;           (3)
     *        read x;           (4)
     *        read x;           (5)
     *      } else {
     *        read x;           (6)
     *        read x;           (7)
     *        read x;           (8)
     *      }
     *   } else {
     *      if (...) {          (9)
     *        read x;           (10)
     *        read x;           (11)
     *        read x;           (12)
     *      } else {
     *        read x;           (13)
     *        read x;           (14)
     *        read x;           (15)
     *      }
     *      read x;             (16)
     *   }
     * }
     * ****************** END OF PROGRAM ******************
     */

    DesignExtractor de(&TestExtractIfStatement::pkb);
    de.extract(&program);
    // Check Next(ifs, s1), Next(ifs, s2) where s1, s2 are the first THEN
    // and ELSE statements respectively
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getId())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getId())
                .count(thenIfStatement.getId()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getId())
                .count(elseIfStatement.getId()));

    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenIfStatement.getId())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenIfStatement.getId())
                .count(thenIfThenStatement1.getId()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenIfStatement.getId())
                .count(thenIfElseStatement1.getId()));

    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseIfStatement.getId())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseIfStatement.getId())
                .count(elseIfThenStatement1.getId()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseIfStatement.getId())
                .count(elseIfElseStatement1.getId()));

    // Check Next(s1, s2) where s1, s2 are consecutive THEN/ELSE statements
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement1.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement1.getId())
                .count(thenIfThenStatement2.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement2.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement2.getId())
                .count(thenIfThenStatement3.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement1.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement1.getId())
                .count(thenIfElseStatement2.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement2.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement2.getId())
                .count(thenIfElseStatement3.getId()));

    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement1.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement1.getId())
                .count(elseIfThenStatement2.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement2.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement2.getId())
                .count(elseIfThenStatement3.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement1.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement1.getId())
                .count(elseIfElseStatement2.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement2.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement2.getId())
                .count(elseIfElseStatement3.getId()));

    // Check Next(s1, s2) where s1 is the last THEN/ELSE statement and s2 is
    // the statement which follows the IF statement
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement3.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement3.getId())
                .count(afterIfStatement.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement3.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement3.getId())
                .count(afterIfStatement.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement3.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement3.getId())
                .count(afterIfStatement.getId()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement3.getId())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement3.getId())
                .count(afterIfStatement.getId()));
}
