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
    static inline int terminalStmtIndex = -2;

public:
    static void reset() {
        ExtractionContext::getInstance().reset();
        terminalStmtIndex = -2;
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
    ProgLineIndex tIndex = TestExtractNextBipRelationship::terminalStmtIndex;
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

    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement1.getId()) == InvalidIndex);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement1.getId())
                .empty());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackToLines(statement1.getId())
                .empty());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement1.getId())
                .empty());
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .count(statement2.getId()) == 1);

    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement2.getId()) == InvalidIndex);
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement2.getId())
                .empty());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackToLines(statement2.getId())
                .empty());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement2.getId())
                .empty());
    // NextBip(
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(tIndex));
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

    statement1.setProcName(procedure2.getId());
    statement2.setVariable(&variable);
    statement3.setVariable(&variable);
    procedure1.addToStmtLst(&statement1);
    procedure1.addToStmtLst(&statement2);
    procedure2.addToStmtLst(&statement3);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    // In reverse order of call dependencies
    ProgLineIndex t2 = TestExtractNextBipRelationship::terminalStmtIndex--;
    ProgLineIndex t1 = TestExtractNextBipRelationship::terminalStmtIndex--;

    DesignExtractor de(&TestExtractNextBipRelationship::pkb);
    de.extract(&program);

    // NextBip(1, 3)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .count(statement3.getId()) == 1);

    // BranchIn from 1 to 3
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement1.getId()) == statement3.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement3.getId())
                .count(statement1.getId()));

    // NextBip(3, t2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .count(t2) == 1);

    // NextBip(t2, 2)
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).size() ==
            1);
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).count(
                statement2.getId()) == 1);

    // BranchBack from t2 to 2
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchBackToLines(t2).count(
        statement2.getId()));
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement2.getId())
                .count(t2));

    // NextBip(2, t1)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(t1) == 1);
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

    statement1.setProcName(procedure2.getId());
    statement2.setVariable(&variable);
    statement3.setProcName(procedure3.getId());
    statement4.setVariable(&variable);
    procedure1.addToStmtLst(&statement1);
    procedure1.addToStmtLst(&statement2);
    procedure2.addToStmtLst(&statement3);
    procedure3.addToStmtLst(&statement4);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);
    program.addToProcLst(&procedure3);

    // In reverse order of call dependencies
    ProgLineIndex t3 = TestExtractNextBipRelationship::terminalStmtIndex--;
    ProgLineIndex t2 = TestExtractNextBipRelationship::terminalStmtIndex--;
    ProgLineIndex t1 = TestExtractNextBipRelationship::terminalStmtIndex--;

    DesignExtractor de(&TestExtractNextBipRelationship::pkb);
    de.extract(&program);

    // NextBip(1, 3)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .count(statement3.getId()) == 1);

    // BranchIn from 1 to 3
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement1.getId()) == statement3.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement3.getId())
                .count(statement1.getId()));

    // NextBip(3, 4)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .count(statement4.getId()));

    // BranchIn from 3 to 4
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement3.getId()) == statement4.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement4.getId())
                .count(statement3.getId()));

    // NextBip(4, t3)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .count(t3) == 1);

    // NextBip(t3, t2)
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t3).size() ==
            1);
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t3).count(t2) ==
            1);

    // BranchBack from t3 to t2
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getBranchBackToLines(t3).count(t2));
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getBranchBackFromLines(t2).count(
            t3));

    // NextBip(t2, 2)
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).size() ==
            1);
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).count(
        statement2.getId()));

    // BranchBack from t2 to 2
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchBackToLines(t2).count(
        statement2.getId()));
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement2.getId())
                .count(t2));

    // NextBip(2, t1)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(t1));

    // NextBip(t1, _)
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t1).empty());
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

    statement1.setProcName(procedure2.getId());
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

    // In reverse order of call dependencies
    ProgLineIndex t2 = TestExtractNextBipRelationship::terminalStmtIndex--;
    ProgLineIndex t1 = TestExtractNextBipRelationship::terminalStmtIndex;

    DesignExtractor de(&TestExtractNextBipRelationship::pkb);
    de.extract(&program);

    // NextBip(1, 3)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .count(statement3.getId()) == 1);

    // BranchIn from 1 to 3
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement1.getId()) == statement3.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement3.getId())
                .count(statement1.getId()));

    // Next(3, 4), NextBip(3, 5)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .size() == 2);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .count(statement4.getId()) == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .count(statement5.getId()) == 1);

    // NextBip(4, t2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .count(t2) == 1);
    // NextBip(5, t2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement5.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement5.getId())
            .count(t2) == 1);

    // NextBip(t2, 2)
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).size() ==
            1);
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).count(
                statement2.getId()) == 1);

    // BranchBack from t2 to 2
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchBackToLines(t2).count(
        statement2.getId()));
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement2.getId())
                .count(t2));

    // NextBip(2, t1)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(t1) == 1);
}

TEST_CASE("TestExtractNextBipRelationship: Correctly extracts when a called "
          "procedure ends with a WhileStatement") {
    TestExtractNextBipRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractNextBipRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractNextBipRelationship::PROC_NAME_2);
    Statement statement1(1, StatementType::CALL);
    Statement statement2(2, StatementType::WHILE);
    Statement statement3(3, StatementType::READ);
    Variable variable(TestExtractNextBipRelationship::VAR_NAME);

    statement1.setProcName(procedure2.getId());
    TestUtil::addConditionalExpression(&statement2);
    statement3.setVariable(&variable);

    procedure1.addToStmtLst(&statement1);
    procedure2.addToStmtLst(&statement2);
    statement2.addThenStmt(&statement3);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    /**
     * THE ABOVE PROGRAM TRANSLATES TO:
     * ****************** START OF PROGRAM ******************
     * procedure PROC_1 {
     *    call PROC_2;           (1)
     * }
     * procedure PROC_2 {
     *   while (x > 0) then {    (2)
     *     read VAR;             (3)
     *   }
     * }
     * ****************** END OF PROGRAM ******************
     */

    // In reverse order of call dependencies
    ProgLineIndex t2 = TestExtractNextBipRelationship::terminalStmtIndex--;
    ProgLineIndex t1 = TestExtractNextBipRelationship::terminalStmtIndex;

    DesignExtractor de(&TestExtractNextBipRelationship::pkb);
    de.extract(&program);

    // NextBip(1, 2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .count(statement2.getId()) == 1);

    // BranchIn from 1 to 2
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement1.getId()) == statement2.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement2.getId())
                .count(statement1.getId()));

    // NextBip(2, 3), NextBip(2, t2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .size() == 2);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(statement3.getId()) == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(t2) == 1);

    // NextBip(3, 2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .count(statement2.getId()) == 1);

    // BranchBack from t2 to t1
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getBranchBackToLines(t2).count(t1));
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getBranchBackFromLines(t1).count(
            t2));
}

TEST_CASE("TestExtractNextBipRelationship: Correctly extracts when call "
          "statement is in while loop") {
    TestExtractNextBipRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractNextBipRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractNextBipRelationship::PROC_NAME_2);
    Statement statement1(1, StatementType::READ);
    Statement statement2(2, StatementType::WHILE);
    Statement statement3(3, StatementType::CALL);
    Statement statement4(4, StatementType::WHILE);
    Statement statement5(5, StatementType::READ);
    Variable variable(TestExtractNextBipRelationship::VAR_NAME);

    statement1.setVariable(&variable);
    TestUtil::addConditionalExpression(&statement2);
    statement3.setProcName(procedure2.getId());
    TestUtil::addConditionalExpression(&statement4);
    statement5.setVariable(&variable);

    procedure1.addToStmtLst(&statement1);
    procedure1.addToStmtLst(&statement2);
    procedure2.addToStmtLst(&statement4);
    statement2.addThenStmt(&statement3);
    statement4.addThenStmt(&statement5);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);

    /**
     * THE ABOVE PROGRAM TRANSLATES TO:
     * ****************** START OF PROGRAM ******************
     * procedure PROC_1 {
     *   read VAR;               (1)
     *   while (x > 0) then {    (2)
     *     call PROC_2;          (3)
     *   }
     * }
     * procedure PROC_2 {
     *   while (x > 0) then {    (4)
     *     read VAR;             (5)
     *   }
     * }
     * ****************** END OF PROGRAM ******************
     */

    // In reverse order of call dependencies
    ProgLineIndex t2 = TestExtractNextBipRelationship::terminalStmtIndex--;
    ProgLineIndex t1 = TestExtractNextBipRelationship::terminalStmtIndex;

    DesignExtractor de(&TestExtractNextBipRelationship::pkb);
    de.extract(&program);

    // NextBip(1, 2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .count(statement2.getId()) == 1);

    // NextBip(2, 3), NextBip(2, t1)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .size() == 2);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(statement3.getId()) == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(t1) == 1);

    // NextBip(3, 4)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .count(statement4.getId()) == 1);

    // BranchIn from 3 to 4
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement3.getId()) == statement4.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement4.getId())
                .count(statement3.getId()));

    // NextBip(4, 5), NextBip(4, t2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .size() == 2);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .count(statement5.getId()) == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .count(t2) == 1);

    // NextBip(5, 4)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement5.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement5.getId())
            .count(statement4.getId()) == 1);

    // NextBip(t2, 2)
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).size() ==
            1);
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).count(
                statement2.getId()) == 1);

    // BranchBack from t2 to 2
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchBackToLines(t2).count(
        statement2.getId()));
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement2.getId())
                .count(t2));
}

TEST_CASE("TestExtractNextBipRelationship: Correctly extracts p1, p2, p3 where "
          "p1 calls p2 and p1, p2 calls p3.") {
    TestExtractNextBipRelationship::reset();

    Program program;
    Procedure procedure1(TestExtractNextBipRelationship::PROC_NAME_1);
    Procedure procedure2(TestExtractNextBipRelationship::PROC_NAME_2);
    Procedure procedure3(TestExtractNextBipRelationship::PROC_NAME_3);
    Statement statement1(1, StatementType::READ);
    Statement statement2(2, StatementType::CALL);
    Statement statement3(3, StatementType::READ);
    Statement statement4(4, StatementType::CALL);
    Statement statement5(5, StatementType::READ);
    Statement statement6(6, StatementType::READ);
    Statement statement7(7, StatementType::CALL);
    Statement statement8(8, StatementType::READ);
    Statement statement9(9, StatementType::IF);
    Statement statement10(10, StatementType::READ);
    Statement statement11(11, StatementType::READ);
    Variable variable(TestExtractNextBipRelationship::VAR_NAME);

    statement1.setVariable(&variable);
    statement2.setProcName(procedure2.getId());
    statement3.setVariable(&variable);
    statement4.setProcName(procedure3.getId());
    statement5.setVariable(&variable);
    statement6.setVariable(&variable);
    statement7.setProcName(procedure3.getId());
    statement8.setVariable(&variable);
    TestUtil::addConditionalExpression(&statement9);
    statement10.setVariable(&variable);
    statement11.setVariable(&variable);

    procedure1.addToStmtLst(&statement1);
    procedure1.addToStmtLst(&statement2);
    procedure1.addToStmtLst(&statement3);
    procedure1.addToStmtLst(&statement4);
    procedure1.addToStmtLst(&statement5);
    procedure2.addToStmtLst(&statement6);
    procedure2.addToStmtLst(&statement7);
    procedure2.addToStmtLst(&statement8);
    procedure3.addToStmtLst(&statement9);
    statement9.addThenStmt(&statement10);
    statement9.addElseStmt(&statement11);
    program.addToProcLst(&procedure1);
    program.addToProcLst(&procedure2);
    program.addToProcLst(&procedure3);

    /**
     * THE ABOVE PROGRAM TRANSLATES TO:
     * ****************** START OF PROGRAM ******************
     * procedure PROC_1 {
     *   read VAR;               (1)
     *   call PROC_2;            (2)
     *   read VAR;               (3)
     *   call PROC_3;            (4)
     *   read VAR;               (5)
     * }
     * procedure PROC_2 {
     *   read VAR;               (6)
     *   call PROC_3;            (7)
     *   read VAR;               (8)
     * }
     *
     * procedure PROC_3 {
     *   if (x > 0) then {       (9)
     *     read VAR;             (10)
     *   } else {
     *     read VAR;             (11)
     *   }
     * }
     * ****************** END OF PROGRAM ******************
     */

    // In reverse order of call dependencies
    ProgLineIndex t3 = TestExtractNextBipRelationship::terminalStmtIndex--;
    ProgLineIndex t2 = TestExtractNextBipRelationship::terminalStmtIndex--;
    ProgLineIndex t1 = TestExtractNextBipRelationship::terminalStmtIndex--;

    DesignExtractor de(&TestExtractNextBipRelationship::pkb);
    de.extract(&program);

    // NextBip(1, 2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement1.getId())
            .count(statement2.getId()) == 1);

    // NextBip(2, 6)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement2.getId())
            .count(statement6.getId()) == 1);

    // BranchIn from 2 to 6
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement2.getId()) == statement6.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement6.getId())
                .count(statement2.getId()));

    // NextBip(3, 4)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement3.getId())
            .count(statement4.getId()) == 1);

    // NextBip(4, 9)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement4.getId())
            .count(statement9.getId()) == 1);

    // BranchIn from 4 to 9
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement4.getId()) == statement9.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement9.getId())
                .count(statement4.getId()));

    // NextBip(5, t1)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement5.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement5.getId())
            .count(t1) == 1);

    // NextBip(6, 7)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement6.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement6.getId())
            .count(statement7.getId()) == 1);

    // NextBip(7, 9)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement7.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement7.getId())
            .count(statement9.getId()) == 1);

    // BranchIn from 7 to 9
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchInToLine(
                statement7.getId()) == statement9.getId());
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchInFromLines(statement9.getId())
                .count(statement7.getId()));

    // NextBip(8, t2)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement8.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement8.getId())
            .count(t2) == 1);

    // NextBip(t2, 3)
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).size() ==
            1);
    REQUIRE(TestExtractNextBipRelationship::pkb.getNextBipLines(t2).count(
                statement3.getId()) == 1);

    // BranchBack from t2 to 3
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getBranchBackToLines(t2).size() ==
        1);
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchBackToLines(t2).count(
        statement3.getId()));
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement3.getId())
                .count(t2));

    // NextBip(9, 10), NextBip(9, 11)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement9.getId())
            .size() == 2);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement9.getId())
            .count(statement10.getId()) == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement9.getId())
            .count(statement11.getId()) == 1);

    // NextBip(10, t3)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement10.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement10.getId())
            .count(t3) == 1);

    // NextBip(11, t3)
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement10.getId())
            .size() == 1);
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getNextBipLines(statement11.getId())
            .count(t3) == 1);

    // BranchBack from t3 to 5, 8
    REQUIRE(
        TestExtractNextBipRelationship::pkb.getBranchBackToLines(t3).size() ==
        2);
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchBackToLines(t3).count(
        statement5.getId()));
    REQUIRE(TestExtractNextBipRelationship::pkb.getBranchBackToLines(t3).count(
        statement8.getId()));
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement5.getId())
                .count(t3));
    REQUIRE(TestExtractNextBipRelationship::pkb
                .getBranchBackFromLines(statement8.getId())
                .count(t3));
}
