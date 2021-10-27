#include <string>
#include <vector>

#include "TestPKBUtils.h"

#include "pkb/PKB.h"

#include "catch.hpp"

using namespace std;

struct TestPKB {
public:
    static vector<string> createItems();
    static vector<Statement> createStatements();
    static vector<Procedure> createProcs();
    static vector<int> createIndices();
    static vector<Variable> createVariables();
    static vector<ConstantValue> createConstants();
};

vector<string> TestPKB::createItems() {
    return vector<string>{"hello", "goodbye", "sayonara"};
}

vector<Procedure> TestPKB::createProcs() {
    return vector<Procedure>{Procedure("Hello"), Procedure("Goodbye"),
                             Procedure("Now")};
}

vector<Statement> TestPKB::createStatements() {
    return vector<Statement>{Statement(1, StatementType::ASSIGN),
                             Statement(2, StatementType::CALL),
                             Statement(3, StatementType::IF)};
}

vector<Variable> TestPKB::createVariables() {
    return vector<Variable>{Variable("var_name_1"), Variable("var_name_2"),
                            Variable("var_name_3")};
}

vector<ConstantValue> TestPKB::createConstants() {
    return vector<ConstantValue>{ConstantValue("0"), ConstantValue("1"),
                                 ConstantValue("2")};
}

vector<int> TestPKB::createIndices() { return vector<int>{0, 1, 2}; }

TEST_CASE("PKB: ctor") {
    PKB pkb;
    REQUIRE(pkb.getAllVars().asVector().empty());
    REQUIRE(pkb.getAllConsts().asVector().empty());
    REQUIRE(pkb.getAllProcs().asVector().empty());
    REQUIRE(pkb.getAllStmts().asVector().empty());
}

TEST_CASE("PKB: insertVar/getAllVars") {
    PKB pkb;

    vector<Variable> vars = TestPKB::createVariables();
    for (Variable v : vars) {
        pkb.insertVar(&v);
    }

    vector<VarName> actual = pkb.getAllVars().asVector();
    set<VarName> matches(actual.begin(), actual.end());

    REQUIRE(actual.size() == vars.size());
    for (auto &v : vars) {
        REQUIRE(matches.count(v.getId()) == 1);
    }
}

TEST_CASE("PKB: insertConst/getAllConsts") {
    PKB pkb;

    vector<ConstantValue> constants = TestPKB::createConstants();
    for (ConstantValue c : constants) {
        pkb.insertConst(&c);
    }

    vector<ConstName> actual = pkb.getAllConsts().asVector();
    set<ConstName> matches(actual.begin(), actual.end());

    REQUIRE(actual.size() == constants.size());
    for (auto &c : constants) {
        REQUIRE(matches.count(c.getId()) == 1);
    }
}

TEST_CASE("PKB: insertProcs/getAllProcs") {
    PKB pkb;

    vector<Procedure> procs = TestPKB::createProcs();
    for (Procedure i : procs) {
        pkb.insertProc(&i);
    }

    vector<ProcName> actual = pkb.getAllProcs().asVector();
    set<ProcName> matches(actual.begin(), actual.end());

    REQUIRE(actual.size() == procs.size());
    for (auto &p : procs) {
        REQUIRE(matches.count(p.getId()) == 1);
    }
}

TEST_CASE("PKB: insertStmt/getAllStmts") {
    PKB pkb;

    vector<Statement> stmts = TestPKB::createStatements();
    for (Statement s : stmts) {
        pkb.insertStmt(&s);
    }

    vector<StmtIndex> actual = pkb.getAllStmts().asVector();
    set<StmtIndex> matches(actual.begin(), actual.end());

    REQUIRE(actual.size() == stmts.size());
    for (auto &s : stmts) {
        REQUIRE(matches.count(s.getId()) == 1);
    }
}

TEST_CASE("PKB: getPrintVariable") {
    int stmtIndex = 1;
    string varName = "x";

    auto stmt = new Statement(stmtIndex, StatementType::PRINT);
    auto var = new Variable(varName);
    stmt->setVariable(var);

    PKB pkb;
    pkb.insertStmt(stmt);

    REQUIRE(pkb.getPrintVariable(stmtIndex) == varName);

    delete stmt;
    delete var;
}

TEST_CASE("PKB: getReadVariable") {
    int stmtIndex = 1;
    string varName = "x";

    auto stmt = new Statement(stmtIndex, StatementType::READ);
    auto var = new Variable(varName);
    stmt->setVariable(var);

    PKB pkb;
    pkb.insertStmt(stmt);

    REQUIRE(pkb.getReadVariable(stmtIndex) == varName);

    delete stmt;
    delete var;
}

TEST_CASE("PKB: getCallProcedure") {
    int stmtIndex = 1;
    string procName = "Example";

    auto stmt = new Statement(stmtIndex, StatementType::CALL);
    stmt->setProcName(procName);

    PKB pkb;
    pkb.insertStmt(stmt);

    REQUIRE(pkb.getCallProcedure(stmtIndex) == procName);

    delete stmt;
}

TEST_CASE("Can handle transitive Follows") {
    PKB pkb;
    vector<Statement> items = TestPKB::createStatements();
    pkb.insertFollows(&items.at(0), &items.at(1));
    pkb.insertFollows(&items.at(1), &items.at(2));

    auto follow1 = pkb.getFollowingStmt(1);
    auto followStar1 = pkb.getFollowingStarStmts(1);
    auto follow2 = pkb.getFollowingStmt(2);
    auto followStar2 = pkb.getFollowingStarStmts(2);
    auto follow3 = pkb.getFollowingStmt(3);
    auto followStar3 = pkb.getFollowingStarStmts(3);

    REQUIRE(follow1 == 2);
    REQUIRE(followStar1.size() == 2);
    REQUIRE(followStar1.count(2));
    REQUIRE(followStar1.count(3));

    REQUIRE(follow2 == 3);
    REQUIRE(followStar2.size() == 1);
    REQUIRE(followStar2.count(3));

    REQUIRE(follow3 == -1);
    REQUIRE(followStar3.empty());
}

TEST_CASE("PKB: partialAssignPattern (with insertAssignPattern)") {
    PKB pkb;

    Statement stmt = TestPKBUtils::createAssignStmt(
            2, "x", TestPKBUtils::EXPRESSION_2);

    pkb.insertStmt(&stmt);
    pkb.insertAssignPattern(&stmt);

    StmtIndex stmtIndex = stmt.getId();
    VarName varName = stmt.getVariable()->getId();

    for (auto &query : TestPKBUtils::QUERIES_2) {
        auto exprList = TestPKBUtils::tokenizePattern(query);
        REQUIRE(pkb.partialAssignPattern(stmtIndex, WILDCARD, exprList));
        REQUIRE(pkb.partialAssignPattern(stmtIndex, varName, exprList));
    }
    REQUIRE(pkb.partialAssignPattern(stmtIndex, varName, TestPKBUtils::WILDCARD_EXPRLIST));
}

TEST_CASE("PKB: exactAssignPattern (with insertAssignPattern)") {
    PKB pkb;

    Statement stmt = TestPKBUtils::createAssignStmt(
            1, "small", TestPKBUtils::EXPRESSION_1);

    pkb.insertStmt(&stmt);
    pkb.insertAssignPattern(&stmt);

    StmtIndex stmtIndex = stmt.getId();
    VarName varName = stmt.getVariable()->getId();

    REQUIRE(pkb.exactAssignPattern(stmtIndex, WILDCARD, TestPKBUtils::INFIX_1));
    REQUIRE(pkb.exactAssignPattern(stmtIndex, varName, TestPKBUtils::INFIX_1));
    REQUIRE(pkb.exactAssignPattern(stmtIndex, varName, TestPKBUtils::WILDCARD_EXPRLIST));
}

TEST_CASE("PKB: getPartialAssignPatternStmts (with insertAssignPattern)") {
    PKB pkb;

    Statement stmt = TestPKBUtils::createAssignStmt(
            1, "x", TestPKBUtils::EXPRESSION_2);

    pkb.insertStmt(&stmt);
    pkb.insertAssignPattern(&stmt);

    StmtIndex stmtIndex = stmt.getId();
    VarName varName = stmt.getVariable()->getId();

    set<StmtIndex> stmts;

    for (auto &query : TestPKBUtils::QUERIES_2) {
        auto exprList = TestPKBUtils::tokenizePattern(query);

        stmts = pkb.getPartialAssignPatternStmts(WILDCARD, exprList);
        REQUIRE(stmts.size() == 1);
        REQUIRE(stmts.count(stmtIndex) == 1);

        stmts = pkb.getPartialAssignPatternStmts(varName, exprList);
        REQUIRE(stmts.size() == 1);
        REQUIRE(stmts.count(stmtIndex) == 1);
    }

    stmts = pkb.getPartialAssignPatternStmts(varName, TestPKBUtils::WILDCARD_EXPRLIST);
    REQUIRE(stmts.size() == 1);
    REQUIRE(stmts.count(stmtIndex) == 1);
}

TEST_CASE("PKB: getExactAssignPatternStmts (with insertAssignPattern)") {
    PKB pkb;

    Statement stmt = TestPKBUtils::createAssignStmt(
            1, "x", TestPKBUtils::EXPRESSION_2);

    pkb.insertStmt(&stmt);
    pkb.insertAssignPattern(&stmt);

    StmtIndex stmtIndex = stmt.getId();
    VarName varName = stmt.getVariable()->getId();

    set<StmtIndex> stmts;

    stmts = pkb.getExactAssignPatternStmts(WILDCARD, TestPKBUtils::INFIX_2);
    REQUIRE(stmts.size() == 1);
    REQUIRE(stmts.count(stmtIndex) == 1);

    stmts = pkb.getExactAssignPatternStmts(varName, TestPKBUtils::INFIX_2);
    REQUIRE(stmts.size() == 1);
    REQUIRE(stmts.count(stmtIndex) == 1);

    stmts = pkb.getExactAssignPatternStmts(varName, TestPKBUtils::WILDCARD_EXPRLIST);
    REQUIRE(stmts.size() == 1);
    REQUIRE(stmts.count(stmtIndex) == 1);
}
