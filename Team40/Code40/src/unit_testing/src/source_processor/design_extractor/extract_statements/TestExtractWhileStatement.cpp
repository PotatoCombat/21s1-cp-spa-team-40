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

TEST_CASE(
    "TestExtractWhileStatement: Correctly extracts a simple WhileStatement") {
    TestExtractWhileStatement::reset();

    Program program;
    Procedure procedure(TestExtractWhileStatement::PROC_NAME);
    Statement whileStatement(1, StatementType::WHILE);
    Statement thenStatement(2, StatementType::READ);
    Variable variable(TestExtractWhileStatement::VAR_NAME);

    whileStatement.setProcName(procedure.getName());
    whileStatement.addThenStmt(&thenStatement);
    whileStatement.addExpressionVar(&variable);
    thenStatement.setVariable(&variable);
    procedure.addToStmtLst(&whileStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractWhileStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractWhileStatement::pkb.getAllProcs().asVector().size() ==
            1);
    REQUIRE(TestExtractWhileStatement::pkb.getAllStmts().asVector().size() ==
            2);
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

    whileStatement.setProcName(procedure.getName());
    whileStatement.addThenStmt(&thenWhileStatement);
    whileStatement.addExpressionVar(&variable);
    thenWhileStatement.addThenStmt(&thenWhileThenStatement);
    thenWhileStatement.addExpressionVar(&variable);
    thenWhileThenStatement.setVariable(&variable);
    procedure.addToStmtLst(&whileStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractWhileStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractWhileStatement::pkb.getAllProcs().asVector().size() ==
            1);
    REQUIRE(TestExtractWhileStatement::pkb.getAllStmts().asVector().size() ==
            3);

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
