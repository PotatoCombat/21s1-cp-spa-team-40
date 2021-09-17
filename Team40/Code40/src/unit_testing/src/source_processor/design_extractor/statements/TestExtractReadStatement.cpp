#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractReadStatement {
    static PKB pkb;
    static ProcName PROC_NAME;
    static VarName VAR_NAME;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractReadStatement::pkb = PKB();
ProcName TestExtractReadStatement::PROC_NAME = "PROC_NAME";
VarName TestExtractReadStatement::VAR_NAME = "VAR";

TEST_CASE(
    "TestExtractReadStatement: Correctly extracts a simple ReadStatement") {
    TestExtractReadStatement::reset();

    Program program;
    Procedure procedure(TestExtractReadStatement::PROC_NAME);
    Statement readStatement(1, StatementType::READ);
    Variable modifiedVar(TestExtractReadStatement::VAR_NAME);

    readStatement.setProcName(procedure.getName());
    readStatement.setVariable(&modifiedVar);
    procedure.addToStmtLst(&readStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractReadStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractReadStatement::pkb.getAllProcs().asVector().size() == 1);
    REQUIRE(TestExtractReadStatement::pkb.getAllStmts().asVector().size() == 1);

    // Check that procedure modifies variable correctly extracted
    REQUIRE(TestExtractReadStatement::pkb
                .getProcsModifyingVar(modifiedVar.getName())
                .count(procedure.getName()));

    // Check that statement modifies variable correctly extracted
    REQUIRE(TestExtractReadStatement::pkb
                .getStmtsModifyingVar(modifiedVar.getName())
                .count(readStatement.getIndex()));
}
