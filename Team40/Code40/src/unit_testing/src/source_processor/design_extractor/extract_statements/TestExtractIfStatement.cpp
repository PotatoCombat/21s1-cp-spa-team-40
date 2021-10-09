#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

/**
 * TODO: Refactor tests once we fix validation for cond_expr
 * https://github.com/nus-cs3203/21s1-cp-spa-team-40/issues/148
 */

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

    ifStatement.setProcName(procedure.getName());
    ifStatement.addThenStmt(&thenStatement);
    ifStatement.addElseStmt(&elseStatement);
    ifStatement.addExpressionVar(&variable);
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
    Statement elseStatement(3, StatementType::READ);
    Statement thenIfThenStatement(5, StatementType::READ);
    Statement thenIfElseStatement(6, StatementType::READ);
    Variable variable(TestExtractIfStatement::VAR_NAME);

    ifStatement.setProcName(procedure.getName());
    ifStatement.addThenStmt(&thenIfStatement);
    ifStatement.addElseStmt(&elseStatement);
    ifStatement.addExpressionVar(&variable);
    elseStatement.setVariable(&variable);
    thenIfStatement.addThenStmt(&thenIfThenStatement);
    thenIfStatement.addElseStmt(&thenIfElseStatement);
    thenIfStatement.addExpressionVar(&variable);
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

    ifStatement.addExpressionVar(&variable);

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

    ifStatement.addExpressionVar(&variable);

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

    // Check Next(s1, s2) where s1, s2 and consecutive THEN/ELSE statements
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
