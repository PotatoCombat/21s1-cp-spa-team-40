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

    addConditionalExpression(&ifStatement);

    ifStatement.setProcName(procedure.getName());
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
                thenStatement.getIndex()) == ifStatement.getIndex());
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                elseStatement.getIndex()) == ifStatement.getIndex());
    REQUIRE(
        TestExtractIfStatement::pkb.getParentStarStmts(thenStatement.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractIfStatement::pkb.getParentStarStmts(thenStatement.getIndex())
            .count(ifStatement.getIndex()));
    REQUIRE(
        TestExtractIfStatement::pkb.getParentStarStmts(elseStatement.getIndex())
            .size() == 1);
    REQUIRE(
        TestExtractIfStatement::pkb.getParentStarStmts(elseStatement.getIndex())
            .count(ifStatement.getIndex()));
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

    addConditionalExpression(&ifStatement);
    addConditionalExpression(&thenIfStatement);

    ifStatement.setProcName(procedure.getName());
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
                thenIfStatement.getIndex()) == ifStatement.getIndex());
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                elseStatement.getIndex()) == ifStatement.getIndex());

    // Check that thenIfStatement is parent of thenIfThenStatement &
    // thenIfElseStatement
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                thenIfThenStatement.getIndex()) == thenIfStatement.getIndex());
    REQUIRE(TestExtractIfStatement::pkb.getParentStmt(
                thenIfElseStatement.getIndex()) == thenIfStatement.getIndex());

    // Check that thenIfThenStatement & thenIfElseStatement have 2
    // transitive parents each
    REQUIRE(TestExtractIfStatement::pkb
                .getParentStarStmts(thenIfThenStatement.getIndex())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb
                .getParentStarStmts(thenIfElseStatement.getIndex())
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

    addConditionalExpression(&ifStatement);

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
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement1.getIndex());
    auto thenFollowStar1 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        thenStatement1.getIndex());
    auto thenFollow2 =
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement2.getIndex());
    auto thenFollowStar2 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        thenStatement2.getIndex());
    auto thenFollow3 =
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement3.getIndex());
    auto thenFollowStar3 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        thenStatement3.getIndex());
    auto elseFollow1 =
        TestExtractIfStatement::pkb.getFollowingStmt(elseStatement1.getIndex());
    auto elseFollowStar1 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        elseStatement1.getIndex());
    auto elseFollow2 =
        TestExtractIfStatement::pkb.getFollowingStmt(elseStatement2.getIndex());
    auto elseFollowStar2 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        elseStatement2.getIndex());
    auto elseFollow3 =
        TestExtractIfStatement::pkb.getFollowingStmt(elseStatement3.getIndex());
    auto elseFollowStar3 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        elseStatement3.getIndex());

    REQUIRE(thenFollow1 == thenStatement2.getIndex());
    REQUIRE(thenFollowStar1.size() == 2);
    REQUIRE(thenFollowStar1.count(thenStatement2.getIndex()));
    REQUIRE(thenFollowStar1.count(thenStatement3.getIndex()));

    REQUIRE(thenFollow2 == thenStatement3.getIndex());
    REQUIRE(thenFollowStar2.size() == 1);
    REQUIRE(thenFollowStar2.count(thenStatement3.getIndex()));

    REQUIRE(thenFollow3 == -1);
    REQUIRE(thenFollowStar3.empty());

    REQUIRE(elseFollow1 == elseStatement2.getIndex());

    REQUIRE(elseFollowStar1.size() == 2);
    REQUIRE(elseFollowStar1.count(elseStatement2.getIndex()));
    REQUIRE(elseFollowStar1.count(elseStatement3.getIndex()));

    REQUIRE(elseFollow2 == elseStatement3.getIndex());
    REQUIRE(elseFollowStar2.size() == 1);
    REQUIRE(elseFollowStar2.count(elseStatement3.getIndex()));

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

    addConditionalExpression(&ifStatement);

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
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getIndex())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getIndex())
                .count(thenStatement1.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getIndex())
                .count(elseStatement1.getIndex()));

    // Check Next(s1, s2) where s1, s2 are consecutive THEN/ELSE statements
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement1.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement1.getIndex())
                .count(thenStatement2.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement2.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement2.getIndex())
                .count(thenStatement3.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement1.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement1.getIndex())
                .count(elseStatement2.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement2.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement2.getIndex())
                .count(elseStatement3.getIndex()));

    // Check Next(s1, s2) where s1 is the last THEN/ELSE statement and s2 is
    // the statement which follows the IF statement
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseStatement3.getIndex())
                .count(afterIfStatement.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenStatement3.getIndex())
                .count(afterIfStatement.getIndex()));
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

    addConditionalExpression(&ifStatement);
    addConditionalExpression(&thenIfStatement);
    addConditionalExpression(&elseIfStatement);

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
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getIndex())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getIndex())
                .count(thenIfStatement.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(ifStatement.getIndex())
                .count(elseIfStatement.getIndex()));

    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenIfStatement.getIndex())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenIfStatement.getIndex())
                .count(thenIfThenStatement1.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(thenIfStatement.getIndex())
                .count(thenIfElseStatement1.getIndex()));

    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseIfStatement.getIndex())
                .size() == 2);
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseIfStatement.getIndex())
                .count(elseIfThenStatement1.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb.getNextLines(elseIfStatement.getIndex())
                .count(elseIfElseStatement1.getIndex()));

    // Check Next(s1, s2) where s1, s2 are consecutive THEN/ELSE statements
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement1.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement1.getIndex())
                .count(thenIfThenStatement2.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement2.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement2.getIndex())
                .count(thenIfThenStatement3.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement1.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement1.getIndex())
                .count(thenIfElseStatement2.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement2.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement2.getIndex())
                .count(thenIfElseStatement3.getIndex()));

    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement1.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement1.getIndex())
                .count(elseIfThenStatement2.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement2.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement2.getIndex())
                .count(elseIfThenStatement3.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement1.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement1.getIndex())
                .count(elseIfElseStatement2.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement2.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement2.getIndex())
                .count(elseIfElseStatement3.getIndex()));

    // Check Next(s1, s2) where s1 is the last THEN/ELSE statement and s2 is
    // the statement which follows the IF statement
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfThenStatement3.getIndex())
                .count(afterIfStatement.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(thenIfElseStatement3.getIndex())
                .count(afterIfStatement.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfThenStatement3.getIndex())
                .count(afterIfStatement.getIndex()));
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractIfStatement::pkb
                .getNextLines(elseIfElseStatement3.getIndex())
                .count(afterIfStatement.getIndex()));
}
