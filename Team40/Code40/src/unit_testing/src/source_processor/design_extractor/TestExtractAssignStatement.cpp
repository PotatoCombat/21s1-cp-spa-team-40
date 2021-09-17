#include "catch.hpp"
#include "common/model/relation/EqualsRelation.h"
#include "common/model/relation/Relation.h"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

// Decided not to stub the PKB here because we'll basically be re-implementing
// the entire thing, since we need to test both reads and writes.
struct TestExtractAssignStatement {
    static PKB pkb;
    static ProcName PROC_NAME;
    static VarName VAR_NAME;
    static vector<string> DUMMY_EXPRESSION_LIST;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractAssignStatement::pkb = PKB();
ProcName TestExtractAssignStatement::PROC_NAME = "PROC";
VarName TestExtractAssignStatement::VAR_NAME = "VAR";
vector<string> TestExtractAssignStatement::DUMMY_EXPRESSION_LIST =
    vector<string>{"1", "+", "2"};

TEST_CASE("TestExtractAssignStatement: Correctly extracts a simple "
          "AssignStatement.") {
    TestExtractAssignStatement::reset();

    Program program;
    Procedure procedure(TestExtractAssignStatement::PROC_NAME);
    Statement statement(1, StatementType::ASSIGN);
    statement.setExpressionLst(
        TestExtractAssignStatement::DUMMY_EXPRESSION_LIST);
    Variable variable(TestExtractAssignStatement::VAR_NAME);

    statement.setVariable(&variable);
    procedure.addToStmtLst(&statement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractAssignStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractAssignStatement::pkb.getAllStmts().asVector().size() ==
            1);
    REQUIRE(TestExtractAssignStatement::pkb
                .getProcsModifyingVar(TestExtractAssignStatement::VAR_NAME)
                .size() == 1);
}

TEST_CASE("DesignExtractor: Follows") {

    TestExtractAssignStatement::reset();

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

        DesignExtractor de(&TestExtractAssignStatement::pkb);
        de.extract(&program);

        auto follow1 = TestExtractAssignStatement::pkb.getFollowingStmt(
            statement1.getIndex());
        auto followStar1 =
            TestExtractAssignStatement::pkb.getFollowingStarStmts(
                statement1.getIndex());
        auto follow2 = TestExtractAssignStatement::pkb.getFollowingStmt(
            statement2.getIndex());
        auto followStar2 =
            TestExtractAssignStatement::pkb.getFollowingStarStmts(
                statement2.getIndex());
        auto follow3 = TestExtractAssignStatement::pkb.getFollowingStmt(
            statement3.getIndex());
        auto followStar3 =
            TestExtractAssignStatement::pkb.getFollowingStarStmts(
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
    TestExtractAssignStatement::pkb = PKB();

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

        DesignExtractor de(&TestExtractAssignStatement::pkb);
        de.extract(&program);

        auto parent1 = TestExtractAssignStatement::pkb.getParentStmt(
            statement1.getIndex());
        auto parentStar1 = TestExtractAssignStatement::pkb.getParentStarStmts(
            statement1.getIndex());
        auto parent2 = TestExtractAssignStatement::pkb.getParentStmt(
            statement2.getIndex());
        auto parentStar2 = TestExtractAssignStatement::pkb.getParentStarStmts(
            statement2.getIndex());
        auto parent3 = TestExtractAssignStatement::pkb.getParentStmt(
            statement3.getIndex());
        auto parentStar3 = TestExtractAssignStatement::pkb.getParentStarStmts(
            statement3.getIndex());

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