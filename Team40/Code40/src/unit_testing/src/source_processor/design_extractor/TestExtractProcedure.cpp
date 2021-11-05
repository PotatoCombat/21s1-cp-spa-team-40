#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractProcedure {
    static PKB pkb;
    static ProcName PROC_NAME;
    static VarName VAR_NAME;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractProcedure::pkb = PKB();
ProcName TestExtractProcedure::PROC_NAME = "PROC_NAME";
ProcName TestExtractProcedure::VAR_NAME = "VAR_NAME";

TEST_CASE("TestExtractProcedure: Correctly throws an error on duplicate "
          "Procedure name.") {
    TestExtractProcedure::reset();

    Program program;
    Procedure proc1(TestExtractProcedure::PROC_NAME);
    Procedure proc2(TestExtractProcedure::PROC_NAME);
    Statement statement1(1, StatementType::READ);
    Statement statement2(2, StatementType::READ);
    Variable variable(TestExtractProcedure::VAR_NAME);

    statement1.setVariable(&variable);
    statement2.setVariable(&variable);
    proc1.addToStmtLst(&statement1);
    proc2.addToStmtLst(&statement2);
    program.addToProcLst(&proc1);
    program.addToProcLst(&proc2);

    DesignExtractor de(&TestExtractProcedure::pkb);
    REQUIRE_THROWS_WITH(de.extract(&program),
                        "Encountered a Procedure with a duplicate name.");
}
