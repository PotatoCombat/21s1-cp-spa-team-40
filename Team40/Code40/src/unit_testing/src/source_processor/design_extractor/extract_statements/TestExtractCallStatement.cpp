#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractCallStatement {
    static PKB pkb;
    static ProcName PROC_NAME_1;
    static ProcName PROC_NAME_2;
    static VarName VAR_NAME;
    static vector<string> DUMMY_EXPRESSION_LIST;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractCallStatement::pkb = PKB();
ProcName TestExtractCallStatement::PROC_NAME_1 = "PROC_NAME_1";
ProcName TestExtractCallStatement::PROC_NAME_2 = "PROC_NAME_2";
VarName TestExtractCallStatement::VAR_NAME = "VAR";
vector<string> TestExtractCallStatement::DUMMY_EXPRESSION_LIST =
    vector<string>{"1", "+", "2"};

TEST_CASE(
    "TestExtractCallStatement: Correctly extracts a simple CallStatement") {
    TestExtractCallStatement::reset();

    Program program;
    Procedure callingProc(TestExtractCallStatement::PROC_NAME_1);
    Procedure calledProc(TestExtractCallStatement::PROC_NAME_2);
    Statement statement(1, StatementType::CALL);

    statement.setProcName(calledProc.getName());
    program.addToProcLst(&callingProc);
    program.addToProcLst(&calledProc);
    callingProc.addToStmtLst(&statement);

    DesignExtractor de(&TestExtractCallStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractCallStatement::pkb.getAllProcs().asVector().size() == 2);
    REQUIRE(TestExtractCallStatement::pkb.getAllStmts().asVector().size() == 1);
    REQUIRE(TestExtractCallStatement::pkb.getAllStmts(StatementType::CALL)
                .asVector()
                .size() == 1);
}

TEST_CASE("TestExtractCallStatement: Correctly handles transitive Modifies in "
          "CallStatements.") {
    TestExtractCallStatement::reset();

    Program program;
    Procedure callingProc(TestExtractCallStatement::PROC_NAME_1);
    Procedure calledProc(TestExtractCallStatement::PROC_NAME_2);
    Statement statement1(1, StatementType::CALL);
    Statement statement2(2, StatementType::ASSIGN);
    Variable variable(TestExtractCallStatement::VAR_NAME);

    statement1.setProcName(calledProc.getName());
    statement2.setVariable(&variable);
    statement2.setExpressionLst(
        TestExtractCallStatement::DUMMY_EXPRESSION_LIST);
    callingProc.addToStmtLst(&statement1);
    calledProc.addToStmtLst(&statement2);
    program.addToProcLst(&callingProc);
    program.addToProcLst(&calledProc);

    DesignExtractor de(&TestExtractCallStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractCallStatement::pkb.getAllStmts().asVector().size() == 2);
    REQUIRE(TestExtractCallStatement::pkb.getAllStmts(StatementType::CALL)
                .asVector()
                .size() == 1);
    REQUIRE(
        TestExtractCallStatement::pkb.getProcsModifyingVar(variable.getName())
            .size() == 2);
}

TEST_CASE("TestExtractCallStatement: Correctly handles transitive Uses in "
          "CallStatements.") {
    TestExtractCallStatement::reset();

    Program program;
    Procedure callingProc(TestExtractCallStatement::PROC_NAME_1);
    Procedure calledProc(TestExtractCallStatement::PROC_NAME_2);
    Statement callStatement(1, StatementType::CALL);
    Statement printStatement(2, StatementType::PRINT);
    Variable variable(TestExtractCallStatement::VAR_NAME);

    callStatement.setProcName(calledProc.getName());
    printStatement.setVariable(&variable);
    callingProc.addToStmtLst(&callStatement);
    calledProc.addToStmtLst(&printStatement);
    program.addToProcLst(&callingProc);
    program.addToProcLst(&calledProc);

    DesignExtractor de(&TestExtractCallStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractCallStatement::pkb.getAllStmts().asVector().size() == 2);
    REQUIRE(TestExtractCallStatement::pkb.getAllStmts(StatementType::CALL)
                .asVector()
                .size() == 1);
    REQUIRE(TestExtractCallStatement::pkb.getProcsUsingVar(variable.getName())
                .size() == 2);
}