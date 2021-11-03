#include "PKBStub.h"

// based on the Example procedure in Test40/sample_source
Iterator<StmtIndex> PKBStub::getAllStmts() {
    vector<int> stmts{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Iterator<StmtIndex> iter(stmts);
    return iter;
}

Iterator<VarName> PKBStub::getAllVars() {
    vector<string> vars{"x", "y", "i", "z"};
    Iterator<VarName> iter(vars);
    return iter;
}

Iterator<ConstName> PKBStub::getAllConsts() {
    vector<ConstName> constants{"1", "2", "3", "5"};
    Iterator<ConstName> iter(constants);
    return iter;
}

Iterator<StmtIndex> PKBStub::getAllStmts(const StatementType &stmtType) {
    vector<int> allStmts = getAllStmts().asVector();
    vector<int> filteredStmts;
    for (auto stmt : allStmts) {
        if (getStmtType(stmt) == stmtType) {
            filteredStmts.push_back(stmt);
        }
    }
    Iterator<StmtIndex> iter(filteredStmts);
    return iter;
}

StatementType PKBStub::getStmtType(const StmtIndex &stmt) {
    vector<StatementType> stmtTypes{
        StatementType::ASSIGN, StatementType::ASSIGN, StatementType::ASSIGN,
        StatementType::WHILE,  StatementType::ASSIGN, StatementType::IF,
        StatementType::ASSIGN, StatementType::ASSIGN, StatementType::ASSIGN,
        StatementType::CALL,   StatementType::ASSIGN, StatementType::CALL};
    return stmtTypes[stmt - 1];
}

StmtIndex PKBStub::getFollowingStmt(const StmtIndex &stmt) {
    vector<int> followingStmts{2, 3, 4, 12, 6, 9, -1, -1, 10, 11, -1, -1};
    return followingStmts[stmt - 1];
}

StmtIndex PKBStub::getPrecedingStmt(const StmtIndex &stmt) {
    vector<int> precedingStmts{-1, 1, 2, 3, -1, 5, -1, -1, 6, 9, 10, 4};
    return precedingStmts[stmt - 1];
}

bool PKBStub::follows(const StmtIndex &stmt1, const StmtIndex &stmt2) {
    return getFollowingStmt(stmt1) == stmt2;
}

set<StmtIndex> PKBStub::getFollowingStarStmts(const StmtIndex &stmt) {
    vector<set<int>> followingStarStmtsList = {
        {2, 3, 4, 12}, {3, 4, 12}, {4, 12}, {12}, {6, 9, 10, 11}, {9, 10, 11}, {}, {}, {10, 11}, {11}, {}, {}};
    return followingStarStmtsList[stmt - 1];
}

set<StmtIndex> PKBStub::getPrecedingStarStmts(const StmtIndex &stmt) {
    vector<set<int>> precedingStarStmtsList = {
        {}, {1}, {1, 2}, {1, 2, 3}, {}, {5}, {}, {}, {5, 6}, {5, 6, 9}, {5, 6, 9, 10}, {1, 2, 3, 4}};
    return precedingStarStmtsList[stmt - 1];
}

bool PKBStub::followsStar(const StmtIndex &stmt1, const StmtIndex &stmt2) {
    set<int> followingStarStmts = getFollowingStarStmts(stmt1);
    return followingStarStmts.find(stmt2) != followingStarStmts.end();
}

StmtIndex PKBStub::getParentStmt(const StmtIndex &stmt) {
    vector<int> parentStmts = {-1, -1, -1, -1, 4, 4, 6, 6, 4, 4, 4, -1};
    return parentStmts[stmt - 1];
}

set<StmtIndex> PKBStub::getChildStmts(const StmtIndex &stmt) {
    vector<set<int>> childStmtsList = {{}, {}, {}, {5, 6, 9, 10, 11}, {}, {7, 8}, {}, {}, {}, {}, {}, {}};
    return childStmtsList[stmt - 1];
}

bool PKBStub::parent(const StmtIndex &stmt1, const StmtIndex &stmt2) {
    return getParentStmt(stmt2) == stmt1;
}

set<StmtIndex> PKBStub::getParentStarStmts(const StmtIndex &stmt) {
    vector<set<int>> parentStarStmtsList = {{}, {}, {}, {}, {4}, {4}, {4, 6}, {4, 6}, {4}, {4}, {4}, {}};
    return parentStarStmtsList[stmt - 1];
}

set<StmtIndex> PKBStub::getChildStarStmts(const StmtIndex &stmt) {
    vector<set<int>> childStarStmtsList = {
        {}, {}, {}, {5, 6, 7, 8, 9, 10, 11}, {}, {7, 8}, {}, {}, {},
        {}, {}, {}};
    return childStarStmtsList[stmt - 1];
}

bool PKBStub::parentStar(const StmtIndex &stmt1, const StmtIndex &stmt2) {
    set<int> childStarStmts = getChildStarStmts(stmt1);
    return find(childStarStmts.begin(), childStarStmts.end(), stmt2) != childStarStmts.end();
}

set<StmtIndex> PKBStub::getStmtsUsingVar(const VarName &var) {
    if (var == "x") {
        return set<int>{4, 5, 6, 7, 8, 9, 10, 12}; // not considered call stmts yet
    } else if (var == "i") {
        return set<int>{4, 9, 11, 12};
    } else if (var == "z") {
        return set<int>{4, 6, 8, 9, 10, 12};
    } else if (var == "y") {
        return set<int>{12};
    } else {
        return set<int>{};
    }
}

set<VarName> PKBStub::getVarsUsedByStmt(const StmtIndex &stmt) {
    vector<set<string>> varsUsed = {{},
                                 {},
                                 {},
                                 {"i", "x", "z"},
                                 {"x"},
                                 {"x", "z"},
                                 {"x"},
                                 {"x", "z"},
                                 {"x", "z", "i"},
                                 {"x", "z"},
                                 {"i"},
                                 {"x", "i", "y", "z"}};
    return varsUsed[stmt - 1];
}

bool PKBStub::stmtUses(const StmtIndex &stmt, const VarName &var) {
    set<string> varsUsed = getVarsUsedByStmt(stmt);
    return varsUsed.find(var) != varsUsed.end();
}

set<StmtIndex> PKBStub::getStmtsModifyingVar(const VarName &var) {
    if (var == "x") {
        return set<int>{1, 4, 5};
    } else if (var == "z") {
        return set<int>{2, 4, 6, 7, 9, 10, 12};
    } else if (var == "i") {
        return set<int>{3, 4, 11, 12};
    } else if (var == "y") {
        return set<int>{4, 8};
    } else {
        return set<int>{};
    }
}

set<VarName> PKBStub::getVarsModifiedByStmt(const StmtIndex &stmt) {
    vector<set<string>> varsModified = {
        {"x"}, {"z"}, {"i"}, {"x", "z", "y", "i"}, {"x"}, {"z", "y"},
        {"z"}, {"y"}, {"z"}, {"z", "x"},           {"i"}, {"x", "z", "i"}};

    return varsModified[stmt - 1];
}

bool PKBStub::stmtModifies(const StmtIndex &stmt, const VarName &var) {
    set<string> varsModified = getVarsModifiedByStmt(stmt);
    return varsModified.find(var) != varsModified.end();
}

VarName PKBStub::getPrintVariable(const StmtIndex &printStmt) {
    // no print in Example procedure
    return "no print";
}

VarName PKBStub::getReadVariable(const StmtIndex &readStmt) {
    // no read in Example procedure
    return "no read";
}

ProcName PKBStub::getCallProcedure(const StmtIndex &callStmt) {
    if (callStmt == 10) {
        return "q";
    } else if (callStmt == 12) {
        return "p";
    } else {
        return "invalid index";
    }
}
