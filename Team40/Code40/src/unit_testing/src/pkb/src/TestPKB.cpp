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
    return vector<Statement>{Statement(0, StatementType::ASSIGN),
                             Statement(1, StatementType::CALL),
                             Statement(2, StatementType::IF)};
}

vector<Variable> TestPKB::createVariables() {
    return vector<Variable>{Variable(0, "var_name_1"),
                            Variable(0, "var_name_1"),
                            Variable(0, "var_name_1")};
}

vector<ConstantValue> TestPKB::createConstants() {
    return vector<ConstantValue>{ConstantValue(0), ConstantValue(1),
                                 ConstantValue(2)};
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

    vector<int> actual = pkb.getAllConsts().asVector();

    for (int i = 0; i < actual.size(); i++) {
        REQUIRE(items.at(i).getValue() == actual.at(i));
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
