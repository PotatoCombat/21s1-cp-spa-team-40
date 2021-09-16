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

TEST_CASE("DesignExtractor: Basic statement parsing") {
    ExtractionContext::getInstance().reset();
    TestDesignExtractor::pkb = PKB();

    SECTION("Correctly extracts AssignStatement.") {
        ProcName PROC_NAME = "PROC";
        VarName ASSIGNED_VAR_NAME = "ASSIGNED_VAR";
        vector<string> DUMMY_EXPRESSION_LIST = vector<string>{"1", "+", "2"};

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

    SECTION("Correctly handles Call Statement") {

        ProcName CALLING_PROC_NAME = "CALLING_PROC";
        ProcName CALLED_PROC_NAME = "CALLED_PROC";

        Program program;
        Procedure procedure1(CALLING_PROC_NAME);
        Procedure procedure2(CALLED_PROC_NAME);
        Statement statement(1, StatementType::CALL);

        statement.setProcName(CALLED_PROC_NAME);
        program.addToProcLst(&procedure1);
        program.addToProcLst(&procedure2);
        procedure1.addToStmtLst(&statement);

        DesignExtractor de(&TestDesignExtractor::pkb);
        de.extract(&program);

        REQUIRE(TestDesignExtractor::pkb.getAllProcs().asVector().size() == 2);
        REQUIRE(TestDesignExtractor::pkb.getAllStmts().asVector().size() == 1);
    }

    SECTION("Correctly handles transitive Modifies in CallStatements.") {
        ProcName CALLED_PROC_NAME = "CALLED_PROC";
        ProcName CALLING_PROC_NAME = "CALLING_PROC";
        VarName ASSIGNED_VAR_NAME = "ASSIGNED_VAR";
        vector<string> DUMMY_EXPRESSION_LIST = vector<string>{"1", "+", "2"};

        Program program;
        Procedure procedure1(CALLING_PROC_NAME);
        Procedure procedure2(CALLED_PROC_NAME);
        Statement statement1(1, StatementType::CALL);
        Statement statement2(2, StatementType::ASSIGN);
        Variable variable(ASSIGNED_VAR_NAME);

        statement1.setProcName(CALLED_PROC_NAME);
        statement2.setVariable(&variable);
        statement2.setExpressionLst(DUMMY_EXPRESSION_LIST);
        procedure1.addToStmtLst(&statement1);
        procedure2.addToStmtLst(&statement2);
        program.addToProcLst(&procedure1);
        program.addToProcLst(&procedure2);

        DesignExtractor de(&TestDesignExtractor::pkb);
        de.extract(&program);

        REQUIRE(TestDesignExtractor::pkb.getAllStmts().asVector().size() == 2);
        REQUIRE(TestDesignExtractor::pkb.getProcsModifyingVar(ASSIGNED_VAR_NAME)
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