#include "DesignExtractorPKBStub.cpp"
#include "catch.hpp"
#include "common/model/relation/EqualsRelation.h"
#include "common/model/relation/Relation.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestDesignExtractor {
    static DesignExtractorPKBStub
};

TEST_CASE("DesignExtractor: Correctly handles Call Statement") {
    ProcName CALLED_PROC_NAME = "CALLED_PROC";
    ProcName CALLING_PROC_NAME = "CALLING_PROC";

    Program program;
    Procedure procedure(CALLING_PROC_NAME);
    Statement statement(1, StatementType::CALL);

    statement.setProcName(CALLED_PROC_NAME);
    program.addToProcLst(procedure);
    procedure.addToStmtLst(statement);

    DesignExtractorPKBStub pkb = DesignExtractorPKBStub();
    DesignExtractor de(&pkb);
    de.extract(program);

    REQUIRE(pkb.getNumProc() == 1);
    REQUIRE(pkb.getNumStmt() == 1);
    REQUIRE(ExtractionContext::getInstance()
                .getProcDependencies(CALLING_PROC_NAME)
                .count(CALLED_PROC_NAME));
}