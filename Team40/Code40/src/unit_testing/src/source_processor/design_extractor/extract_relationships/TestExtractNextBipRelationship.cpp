#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractNextBipRelationship {
    static PKB pkb;
    static ProcName PROC_NAME_1;
    static ProcName PROC_NAME_2;
    static VarName VAR_NAME;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        pkb = PKB();
    }
};

PKB TestExtractNextBipRelationship::pkb = PKB();
ProcName TestExtractNextBipRelationship::PROC_NAME_1 = "PROC_1";
ProcName TestExtractNextBipRelationship::PROC_NAME_2 = "PROC_2";
VarName TestExtractNextBipRelationship::VAR_NAME = "VAR";

TEST_CASE("TestExtractNextBipRelationship: Correctly extracts a simple "
          "NextBip(s1, s2) where s1, s2 are in the same procedure.") {
    TestExtractNextBipRelationship::reset();

    Program program;
    Procedure procedure(TestExtractNextBipRelationship::PROC_NAME_1);
    Statement statement1(1, StatementType::READ);
    Statement statement2(2, StatementType::READ);
    Variable variable(TestExtractNextBipRelationship::VAR_NAME);

    statement1.setVariable(&variable);
    statement2.setVariable(&variable);
    procedure.addToStmtLst(&statement1);
    procedure.addToStmtLst(&statement2);
    program.addToProcLst(&procedure);

    DesignExtractor de(&TestExtractNextBipRelationship::pkb);
    de.extract(&program);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement1.getIndex())
                .size() == 1);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement1.getIndex())
                .count(statement2.getIndex()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement2.getIndex())
                .empty());
}

TEST_CASE("TestExtractNextBipRelationship: Correctly extracts a simple "
          "NextBip(s1, s3), NextBip(s3, s2) where <s1, s2> and <s3> are in "
          "different procedures.") {
    TestExtractNextBipRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractNextBipRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractNextBipRelationship::PROC_NAME_2);
    Statement statement1(1, StatementType::CALL);
    Statement statement2(2, StatementType::READ);
    Statement statement3(3, StatementType::READ);
    Variable variable(TestExtractNextBipRelationship::VAR_NAME);

    statement1.setProcName(procedure2.getName());
    statement2.setVariable(&variable);
    statement3.setVariable(&variable);
    procedure1.addToStmtLst(&statement1);
    procedure1.addToStmtLst(&statement2);
    procedure2.addToStmtLst(&statement3);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    DesignExtractor de(&TestExtractNextBipRelationship::pkb);
    de.extract(&program);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement1.getIndex())
                .size() == 1);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement1.getIndex())
                .count(statement3.getIndex()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement3.getIndex())
                .size() == 1);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement3.getIndex())
                .count(statement2.getIndex()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement2.getIndex())
                .empty());
}
