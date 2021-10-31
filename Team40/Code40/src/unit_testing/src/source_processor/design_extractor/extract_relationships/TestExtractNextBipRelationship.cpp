#include "../TestUtil.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/DesignExtractor.h"

struct TestExtractNextBipRelationship {
    static PKB pkb;
    static ProcName PROC_NAME_1;
    static ProcName PROC_NAME_2;
    static ProcName PROC_NAME_3;
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
ProcName TestExtractNextBipRelationship::PROC_NAME_3 = "PROC_3";
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

TEST_CASE("TestExtractNextBipRelationship: Correctly extracts p1, p2, p3 where "
          "p1 calls p2 and p2 calls p3") {
    TestExtractNextBipRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractNextBipRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractNextBipRelationship::PROC_NAME_2);
    Procedure procedure3(TestExtractNextBipRelationship::PROC_NAME_3);
    Statement statement1(1, StatementType::CALL);
    Statement statement2(2, StatementType::READ);
    Statement statement3(3, StatementType::CALL);
    Statement statement4(4, StatementType::READ);
    Variable variable(TestExtractNextBipRelationship::VAR_NAME);

    statement1.setProcName(procedure2.getName());
    statement2.setVariable(&variable);
    statement3.setProcName(procedure3.getName());
    statement4.setVariable(&variable);
    procedure1.addToStmtLst(&statement1);
    procedure1.addToStmtLst(&statement2);
    procedure2.addToStmtLst(&statement3);
    procedure3.addToStmtLst(&statement4);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);
    program.addToProcLst(&procedure3);

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
                .count(statement4.getIndex()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement4.getIndex())
                .size() == 1);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement4.getIndex())
                .count(statement2.getIndex()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement2.getIndex())
                .empty());
}
TEST_CASE("TestExtractNextBipRelationship: Correctly extracts when a called "
          "procedure ends with an IfStatement") {
    TestExtractNextBipRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractNextBipRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractNextBipRelationship::PROC_NAME_2);
    Statement statement1(1, StatementType::CALL);
    Statement statement2(2, StatementType::READ);
    Statement statement3(3, StatementType::IF);
    Statement statement4(4, StatementType::READ);
    Statement statement5(5, StatementType::READ);
    Variable variable(TestExtractNextBipRelationship::VAR_NAME);

    statement1.setProcName(procedure2.getName());
    statement2.setVariable(&variable);
    TestUtil::addConditionalExpression(&statement3);
    statement4.setVariable(&variable);
    statement5.setVariable(&variable);
    statement3.addThenStmt(&statement4);
    statement3.addElseStmt(&statement5);
    procedure1.addToStmtLst(&statement1);
    procedure1.addToStmtLst(&statement2);
    procedure2.addToStmtLst(&statement3);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    /**
     * THE ABOVE PROGRAM TRANSLATES TO:
     * ****************** START OF PROGRAM ******************
     * procedure PROC_1 {
     *    call PROC_2;           (1)
     *    read x;                (2)
     * }
     * procedure PROC_2 {
     *   if (x > 0) then {       (3)
     *     read VAR;             (4)
     *   } else {
     *     read VAR;             (5)
     *   }
     * }
     * ****************** END OF PROGRAM ******************
     */

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
                .size() == 2);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement3.getIndex())
                .count(statement4.getIndex()) == 1);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement3.getIndex())
                .count(statement5.getIndex()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement4.getIndex())
                .size() == 1);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement4.getIndex())
                .count(statement2.getIndex()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement5.getIndex())
                .size() == 1);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement5.getIndex())
                .count(statement2.getIndex()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb
                .getNextBipLines(statement2.getIndex())
                .empty());
}
