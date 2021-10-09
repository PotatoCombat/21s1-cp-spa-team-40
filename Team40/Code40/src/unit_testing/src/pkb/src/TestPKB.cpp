#include <string>
#include <vector>

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
    return vector<Variable>{Variable("var_name_1"), Variable("var_name_1"),
                            Variable("var_name_1")};
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
    for (Variable i : vars) {
        pkb.insertVar(&i);
    }

    vector<VarName> actual = pkb.getAllVars().asVector();

    for (int i = 0; i < vars.size(); i++) {
        REQUIRE(vars.at(i).getName() == actual.at(i));
    }
}

TEST_CASE("PKB: insertConst/getAllConsts") {
    PKB pkb;

    vector<ConstantValue> items = TestPKB::createConstants();
    for (ConstantValue i : items) {
        pkb.insertConst(&i);
    }

    vector<ConstName> actual = pkb.getAllConsts().asVector();

    for (int i = 0; i < actual.size(); i++) {
        REQUIRE(items.at(i).getName() == actual.at(i));
    }
}

TEST_CASE("PKB: insertProcs/getAllProcs") {
    PKB pkb;

    vector<Procedure> procs = TestPKB::createProcs();
    for (Procedure i : procs) {
        pkb.insertProc(&i);
    }

    vector<ProcName> actual = pkb.getAllProcs().asVector();

    for (int i = 0; i < actual.size(); i++) {
        REQUIRE(procs.at(i).getName() == actual.at(i));
    }
}

TEST_CASE("PKB: insertStmt/getAllStmts") {
    PKB pkb;

    vector<Statement> items = TestPKB::createStatements();
    for (Statement i : items) {
        pkb.insertStmt(&i);
    }

    vector<StmtIndex> test = vector<int>{1, 2, 3};
    vector<StmtIndex> actual = pkb.getAllStmts().asVector();

    for (int i = 0; i < actual.size(); i++) {
        REQUIRE(test.at(i) == actual.at(i));
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
