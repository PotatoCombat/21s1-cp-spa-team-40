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
