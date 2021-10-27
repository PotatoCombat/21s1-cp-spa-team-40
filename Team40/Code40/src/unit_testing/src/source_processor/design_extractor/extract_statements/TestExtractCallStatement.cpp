#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractCallStatement {
    static PKB pkb;
    static ProcName PROC_NAME_1;
    static ProcName PROC_NAME_2;
    static ProcName PROC_NAME_3;
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
ProcName TestExtractCallStatement::PROC_NAME_3 = "PROC_NAME_3";
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

    statement.setProcName(calledProc.getId());
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
    REQUIRE(TestExtractCallStatement::pkb.getCalledProcs(callingProc.getId())
                .count(calledProc.getId()));
    REQUIRE(TestExtractCallStatement::pkb.getCallerProcs(calledProc.getId())
                .count(callingProc.getId()));
    REQUIRE(
        TestExtractCallStatement::pkb.getCalledStarProcs(callingProc.getId())
            .count(calledProc.getId()));
    REQUIRE(
        TestExtractCallStatement::pkb.getCallerStarProcs(calledProc.getId())
            .count(callingProc.getId()));
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

    statement1.setProcName(calledProc.getId());
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
        TestExtractCallStatement::pkb.getProcsModifyingVar(variable.getId())
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

    callStatement.setProcName(calledProc.getId());
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
    REQUIRE(TestExtractCallStatement::pkb.getProcsUsingVar(variable.getId())
                .size() == 2);
}

TEST_CASE("TestExtractCallStatement: Correctly handles transitive Calls in "
          "CallStatements.") {
    TestExtractCallStatement::reset();

    Program program;
    Procedure callingProc(TestExtractCallStatement::PROC_NAME_1);
    Procedure calledCallingProc(TestExtractCallStatement::PROC_NAME_2);
    Procedure calledCalledProc(TestExtractCallStatement::PROC_NAME_3);
    Statement callStatement1(1, StatementType::CALL);
    Statement callStatement2(2, StatementType::CALL);
    Statement printStatement(3, StatementType::PRINT);
    Variable variable(TestExtractCallStatement::VAR_NAME);

    callStatement1.setProcName(calledCallingProc.getId());
    callStatement2.setProcName(calledCalledProc.getId());
    printStatement.setVariable(&variable);
    callingProc.addToStmtLst(&callStatement1);
    calledCallingProc.addToStmtLst(&callStatement2);
    calledCalledProc.addToStmtLst(&printStatement);
    program.addToProcLst(&callingProc);
    program.addToProcLst(&calledCallingProc);
    program.addToProcLst(&calledCalledProc);

    DesignExtractor de(&TestExtractCallStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractCallStatement::pkb.getAllStmts().asVector().size() == 3);
    REQUIRE(TestExtractCallStatement::pkb.getAllStmts(StatementType::CALL)
                .asVector()
                .size() == 2);
    REQUIRE(TestExtractCallStatement::pkb.getCalledProcs(callingProc.getId())
                .count(calledCallingProc.getId()));
    REQUIRE(TestExtractCallStatement::pkb
                .getCallerProcs(calledCallingProc.getId())
                .count(callingProc.getId()));
    REQUIRE(TestExtractCallStatement::pkb
                .getCalledProcs(calledCallingProc.getId())
                .count(calledCalledProc.getId()));
    REQUIRE(
        TestExtractCallStatement::pkb.getCallerProcs(calledCalledProc.getId())
            .count(calledCallingProc.getId()));

    REQUIRE(
        TestExtractCallStatement::pkb.getCalledStarProcs(callingProc.getId())
            .count(calledCalledProc.getId()));
    REQUIRE(TestExtractCallStatement::pkb
                .getCallerStarProcs(calledCalledProc.getId())
                .count(callingProc.getId()));
}
