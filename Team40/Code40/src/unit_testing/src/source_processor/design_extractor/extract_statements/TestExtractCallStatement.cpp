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
    REQUIRE(TestExtractCallStatement::pkb.getCalledProcs(callingProc.getName())
                .count(calledProc.getName()));
    REQUIRE(TestExtractCallStatement::pkb.getCallerProcs(calledProc.getName())
                .count(callingProc.getName()));
    REQUIRE(
        TestExtractCallStatement::pkb.getCalledStarProcs(callingProc.getName())
            .count(calledProc.getName()));
    REQUIRE(
        TestExtractCallStatement::pkb.getCallerStarProcs(calledProc.getName())
            .count(callingProc.getName()));
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

    callStatement1.setProcName(calledCallingProc.getName());
    callStatement2.setProcName(calledCalledProc.getName());
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
    REQUIRE(TestExtractCallStatement::pkb.getCalledProcs(callingProc.getName())
                .count(calledCallingProc.getName()));
    REQUIRE(TestExtractCallStatement::pkb
                .getCallerProcs(calledCallingProc.getName())
                .count(callingProc.getName()));
    REQUIRE(TestExtractCallStatement::pkb
                .getCalledProcs(calledCallingProc.getName())
                .count(calledCalledProc.getName()));
    REQUIRE(
        TestExtractCallStatement::pkb.getCallerProcs(calledCalledProc.getName())
            .count(calledCallingProc.getName()));

    REQUIRE(
        TestExtractCallStatement::pkb.getCalledStarProcs(callingProc.getName())
            .count(calledCalledProc.getName()));
    REQUIRE(TestExtractCallStatement::pkb
                .getCallerStarProcs(calledCalledProc.getName())
                .count(callingProc.getName()));
}
