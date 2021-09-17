#include "DesignExtractorPKBStub.cpp"
#include "catch.hpp"
#include "common/model/relation/EqualsRelation.h"
#include "common/model/relation/Relation.h"
#include "source_processor/design_extractor/DesignExtractor.h"

// Decided not to stub the PKB here because we'll basically be re-implementing
// the entire thing, since we need to test both reads and writes.
struct TestDesignExtractor {
    static PKB pkb;
};

PKB TestDesignExtractor::pkb = PKB();

TEST_CASE("DesignExtractor: Handle AssignStatement") {
    ExtractionContext::getInstance().reset();
    TestDesignExtractor::pkb = PKB();

    ProcName PROC_NAME = "PROC";
    VarName ASSIGNED_VAR_NAME = "ASSIGNED_VAR";
    vector<string> DUMMY_EXPRESSION_LIST = vector<string>{"1", "+", "2"};

    SECTION("Correctly extracts a simple AssignStatement.") {
        Program program;
        Procedure procedure(PROC_NAME);
        Statement statement(1, StatementType::ASSIGN);
        statement.setExpressionLst(DUMMY_EXPRESSION_LIST);
        Variable variable(ASSIGNED_VAR_NAME);

        statement.setVariable(&variable);
        procedure.addToStmtLst(&statement);
        program.addToProcLst(&procedure);

        DesignExtractor de(&TestDesignExtractor::pkb);
        de.extract(&program);

        REQUIRE(TestDesignExtractor::pkb.getAllStmts().asVector().size() == 1);
        REQUIRE(TestDesignExtractor::pkb.getProcsModifyingVar(ASSIGNED_VAR_NAME)
                    .size() == 1);
    }
}

TEST_CASE("DesignExtractor: Handle CallStatement") {
    ExtractionContext::getInstance().reset();
    TestDesignExtractor::pkb = PKB();

    ProcName PROC_NAME_1 = "PROC_1";
    ProcName PROC_NAME_2 = "PROC_2";
    VarName VAR_NAME = "VAR";
    vector<string> DUMMY_EXPRESSION_LIST = vector<string>{"1", "+", "2"};

    SECTION("Correctly extracts a simple CallStatement") {
        Program program;
        Procedure callingProc(PROC_NAME_1);
        Procedure calledProc(PROC_NAME_2);
        Statement statement(1, StatementType::CALL);

        statement.setProcName(calledProc.getName());
        program.addToProcLst(&callingProc);
        program.addToProcLst(&calledProc);
        callingProc.addToStmtLst(&statement);

        DesignExtractor de(&TestDesignExtractor::pkb);
        de.extract(&program);

        REQUIRE(TestDesignExtractor::pkb.getAllProcs().asVector().size() == 2);
        REQUIRE(TestDesignExtractor::pkb.getAllStmts().asVector().size() == 1);
    }

    SECTION("Correctly handles transitive Modifies in CallStatements.") {
        Program program;
        Procedure callingProc(PROC_NAME_1);
        Procedure calledProc(PROC_NAME_2);
        Statement statement1(1, StatementType::CALL);
        Statement statement2(2, StatementType::ASSIGN);
        Variable variable(VAR_NAME);

        statement1.setProcName(calledProc.getName());
        statement2.setVariable(&variable);
        statement2.setExpressionLst(DUMMY_EXPRESSION_LIST);
        callingProc.addToStmtLst(&statement1);
        calledProc.addToStmtLst(&statement2);
        program.addToProcLst(&callingProc);
        program.addToProcLst(&calledProc);

        DesignExtractor de(&TestDesignExtractor::pkb);
        de.extract(&program);

        REQUIRE(TestDesignExtractor::pkb.getAllStmts().asVector().size() == 2);
        REQUIRE(
            TestDesignExtractor::pkb.getProcsModifyingVar(variable.getName())
                .size() == 2);
    }

    SECTION("Correctly handles transitive Uses in CallStatements.") {
        Program program;
        Procedure callingProc(PROC_NAME_1);
        Procedure calledProc(PROC_NAME_2);
        Statement callStatement(1, StatementType::CALL);
        Statement printStatement(2, StatementType::PRINT);
        Variable variable(VAR_NAME);

        callStatement.setProcName(calledProc.getName());
        printStatement.setVariable(&variable);
        callingProc.addToStmtLst(&callStatement);
        calledProc.addToStmtLst(&printStatement);
        program.addToProcLst(&callingProc);
        program.addToProcLst(&calledProc);

        DesignExtractor de(&TestDesignExtractor::pkb);
        de.extract(&program);

        REQUIRE(TestDesignExtractor::pkb.getAllStmts().asVector().size() == 2);
        REQUIRE(TestDesignExtractor::pkb.getProcsUsingVar(variable.getName())
                    .size() == 2);
    }
}

TEST_CASE("DesignExtractor: Follows") {

    ExtractionContext::getInstance().reset();
    TestDesignExtractor::pkb = PKB();

    SECTION("Correctly extracts Follows") {
        ProcName PROC_NAME = "PROC";
        VarName VAR_NAME = "VAR";

        Program program;
        Procedure procedure(PROC_NAME);
        Statement statement1(1, StatementType::READ);
        Statement statement2(2, StatementType::READ);
        Statement statement3(3, StatementType::READ);
        Variable variable(VAR_NAME);

        statement1.setVariable(&variable);
        statement2.setVariable(&variable);
        statement3.setVariable(&variable);
        procedure.addToStmtLst(&statement1);
        procedure.addToStmtLst(&statement2);
        procedure.addToStmtLst(&statement3);
        program.addToProcLst(&procedure);

        DesignExtractor de(&TestDesignExtractor::pkb);
        de.extract(&program);

        auto follow1 =
            TestDesignExtractor::pkb.getFollowingStmt(statement1.getIndex());
        auto followStar1 = TestDesignExtractor::pkb.getFollowingStarStmts(
            statement1.getIndex());
        auto follow2 =
            TestDesignExtractor::pkb.getFollowingStmt(statement2.getIndex());
        auto followStar2 = TestDesignExtractor::pkb.getFollowingStarStmts(
            statement2.getIndex());
        auto follow3 =
            TestDesignExtractor::pkb.getFollowingStmt(statement3.getIndex());
        auto followStar3 = TestDesignExtractor::pkb.getFollowingStarStmts(
            statement3.getIndex());

        REQUIRE(follow1 == statement2.getIndex());
        REQUIRE(followStar1.size() == 2);
        REQUIRE(followStar1.count(statement2.getIndex()));
        REQUIRE(followStar1.count(statement3.getIndex()));

        REQUIRE(follow2 == statement3.getIndex());
        REQUIRE(followStar2.size() == 1);
        REQUIRE(followStar2.count(statement3.getIndex()));

        REQUIRE(follow3 == -1);
        REQUIRE(followStar3.empty());
    }
}
TEST_CASE("DesignExtractor: Parent") {

    ExtractionContext::getInstance().reset();
    TestDesignExtractor::pkb = PKB();

    SECTION("Correctly extracts Parent") {
        vector<string> DUMMY_EXPRESSION_LIST = vector<string>{"1", "=", "2"};
        ProcName PROC_NAME = "PROC";
        VarName VAR_NAME = "VAR";

        Program program;
        Procedure procedure(PROC_NAME);
        Statement statement1(1, StatementType::WHILE);
        Statement statement2(2, StatementType::WHILE);
        Statement statement3(3, StatementType::READ);
        Variable variable(VAR_NAME);

        statement1.setExpressionLst(DUMMY_EXPRESSION_LIST);
        statement1.addThenStmt(&statement2);
        statement2.setExpressionLst(DUMMY_EXPRESSION_LIST);
        statement2.addThenStmt(&statement3);
        statement3.setVariable(&variable);
        procedure.addToStmtLst(&statement1);
        program.addToProcLst(&procedure);

        DesignExtractor de(&TestDesignExtractor::pkb);
        de.extract(&program);

        auto parent1 =
            TestDesignExtractor::pkb.getParentStmt(statement1.getIndex());
        auto parentStar1 =
            TestDesignExtractor::pkb.getParentStarStmts(statement1.getIndex());
        auto parent2 =
            TestDesignExtractor::pkb.getParentStmt(statement2.getIndex());
        auto parentStar2 =
            TestDesignExtractor::pkb.getParentStarStmts(statement2.getIndex());
        auto parent3 =
            TestDesignExtractor::pkb.getParentStmt(statement3.getIndex());
        auto parentStar3 =
            TestDesignExtractor::pkb.getParentStarStmts(statement3.getIndex());

        REQUIRE(parent1 == -1);
        REQUIRE(parentStar1.empty());

        REQUIRE(parent2 == statement1.getIndex());
        REQUIRE(parentStar2.size() == 1);
        REQUIRE(parentStar2.count(statement1.getIndex()));

        REQUIRE(parent3 == statement2.getIndex());
        REQUIRE(parentStar3.size() == 2);
        REQUIRE(parentStar3.count(statement1.getIndex()));
        REQUIRE(parentStar3.count(statement2.getIndex()));
    }
}