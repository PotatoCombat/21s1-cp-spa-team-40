#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

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