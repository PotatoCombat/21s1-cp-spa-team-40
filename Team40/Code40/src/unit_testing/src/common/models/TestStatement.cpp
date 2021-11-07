#include "TestCommonUtils.h"

#include "common/model/Statement.h"

TEST_CASE("Statement: ==") {
    int index = 1;
    VarName varName = TestCommonUtils::VARNAME_1;
    ProcName procName = TestCommonUtils::PROCNAME_1;
    string exprLst = TestCommonUtils::EXPRLST_1;

    auto print1 = TestCommonUtils::createPrintStmt(index, varName);
    auto print2 = TestCommonUtils::createPrintStmt(index, varName);
    REQUIRE(print1 == print2);

    auto read1 = TestCommonUtils::createReadStmt(index, varName);
    auto read2 = TestCommonUtils::createReadStmt(index, varName);
    REQUIRE(read1 == read2);

    auto call1 = TestCommonUtils::createCallStmt(index, procName);
    auto call2 = TestCommonUtils::createCallStmt(index, procName);
    REQUIRE(call1 == call2);

    auto assign1 = TestCommonUtils::createAssignStmt(index, varName, exprLst);
    auto assign2 = TestCommonUtils::createAssignStmt(index, varName, exprLst);
    REQUIRE(assign1 == assign2);

    auto if1 = TestCommonUtils::createIfStmt(index, exprLst);
    auto if2 = TestCommonUtils::createIfStmt(index, exprLst);
    if1.addThenStmt(&print1);
    if2.addThenStmt(&print2);
    if1.addElseStmt(&read1);
    if2.addElseStmt(&read2);
    REQUIRE(if1 == if2);

    auto while1 = TestCommonUtils::createWhileStmt(index, exprLst);
    auto while2 = TestCommonUtils::createWhileStmt(index, exprLst);
    while1.addThenStmt(&call1);
    while2.addThenStmt(&call2);
    while1.addThenStmt(&assign1);
    while2.addThenStmt(&assign2);
    REQUIRE(while1 == while2);
}
