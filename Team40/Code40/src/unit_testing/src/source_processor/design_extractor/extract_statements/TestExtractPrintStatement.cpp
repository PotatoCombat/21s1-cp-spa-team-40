#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractPrintStatement {
    static PKB pkb;
    static ProcName PROC_NAME;
    static VarName VAR_NAME;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractPrintStatement::pkb = PKB();
ProcName TestExtractPrintStatement::PROC_NAME = "PROC_NAME";
VarName TestExtractPrintStatement::VAR_NAME = "VAR";

TEST_CASE(
    "TestExtractPrintStatement: Correctly extracts a simple PrintStatement") {
    TestExtractPrintStatement::reset();

    Program program;
    Procedure procedure(TestExtractPrintStatement::PROC_NAME);
    Statement printStatement(1, StatementType::PRINT);
    Variable printVar(TestExtractPrintStatement::VAR_NAME);

    printStatement.setProcName(procedure.getName());
    printStatement.setVariable(&printVar);
    procedure.addToStmtLst(&printStatement);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractPrintStatement::pkb);
    de.extract(&program);

    REQUIRE(TestExtractPrintStatement::pkb.getAllProcs().asVector().size() ==
            1);
    REQUIRE(TestExtractPrintStatement::pkb.getAllStmts().asVector().size() ==
            1);
    REQUIRE(TestExtractPrintStatement::pkb.getAllStmts(StatementType::PRINT)
                .asVector()
                .size() == 1);

    // Check that procedure modifies variable correctly extracted
    REQUIRE(TestExtractPrintStatement::pkb.getProcsUsingVar(printVar.getName())
                .count(procedure.getName()));

    // Check that statement modifies variable correctly extracted
    REQUIRE(TestExtractPrintStatement::pkb.getStmtsUsingVar(printVar.getName())
                .count(printStatement.getIndex()));
}
