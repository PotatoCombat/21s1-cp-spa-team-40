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
          "in THEN StatementList") {
    TestExtractIfStatement::reset();

    Program program;
    Procedure procedure(TestExtractIfStatement::PROC_NAME);
    Statement whileStatement(1, StatementType::WHILE);
    Statement thenStatement1(2, StatementType::READ);
    Statement thenStatement2(3, StatementType::READ);
    Statement thenStatement3(4, StatementType::READ);
    Variable variable(TestExtractIfStatement::VAR_NAME);

    whileStatement.addExpressionVar(&variable);
    whileStatement.addThenStmt(&thenStatement1);
    whileStatement.addThenStmt(&thenStatement2);
    thenStatement1.setVariable(&variable);
    thenStatement2.setVariable(&variable);
    thenStatement3.setVariable(&variable);
    procedure.addToStmtLst(&whileStatement);
    procedure.addToStmtLst(&thenStatement1);
    procedure.addToStmtLst(&thenStatement2);
    procedure.addToStmtLst(&thenStatement3);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractIfStatement::pkb);
    de.extract(&program);

    auto follow1 =
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement1.getIndex());
    auto followStar1 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        thenStatement1.getIndex());
    auto follow2 =
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement2.getIndex());
    auto followStar2 = TestExtractIfStatement::pkb.getFollowingStarStmts(
        thenStatement2.getIndex());
    auto follow3 =
        TestExtractIfStatement::pkb.getFollowingStmt(thenStatement3.getIndex());
    auto followStar3 = TestExtractIfStatement::pkb.getFollowingStarStmts(
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
