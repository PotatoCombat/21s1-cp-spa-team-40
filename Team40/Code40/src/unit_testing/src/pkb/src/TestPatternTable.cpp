#include <set>
#include <string>
#include <vector>

#include "pkb/PatternTable.h"

#include "catch.hpp"

using namespace std;

struct TestPatterns {
    inline static vector<string> createExprList1() {
        // ((x + 10) * (y - z) / 5)
        return {
            "(", "(", "x", "+", "10", ")", "*", "(",
            "y", "-", "z", ")", "/",  "5", ")",
        };
    }

    inline static vector<string> createPatterns1() {
        // ((x + 10) * (y - z) / 5)
        return {
            "x",
            "10",
            "x,10+",
            "y",
            "z",
            "y,z-",
            "x,10+,y,z-*",
            "5",
            "x,10+,y,z-*,5/",
        };
    }

    inline static string createExactPattern1() {
        // ((x + 10) * (y - z) / 5)
        return "x,10+,y,z-*,5/";
    }

    inline static vector<string> createExprList2() {
        // v + x * y + z * t
        return {
            "v", "+", "x", "*", "y", "+", "z", "*", "t",
        };
    }

    inline static vector<string> createPatterns2() {
        // v + x * y + z * t
        return {
            "v",
            "x",
            "y",
            "x,y*",
            "v,x,y*+",
            "z",
            "t",
            "z,t*",
            "v,x,y*+,z,t*+",
        };
    }

    inline static string createExactPattern2() {
        // v + x * y + z * t
        return "v,x,y*+,z,t*+";
    }

    inline static Statement *createStatement1() {
        auto *stmt = new Statement(1, StatementType::ASSIGN);
        auto *var = new Variable("x");
        stmt->setVariable(var);
        stmt->setExpressionLst(TestPatterns::createExprList1());
        return stmt;
    }
};

TEST_CASE("PatternTable: createPostfix -> createPatterns") {
    vector<string> exprList = TestPatterns::createExprList2();
    vector<string> postfix = PatternTable::createPostfix(exprList);

    vector<string> testPatterns = TestPatterns::createPatterns2();
    vector<string> actualPatterns = PatternTable::createPatterns(postfix);

    REQUIRE(testPatterns.size() == actualPatterns.size());

    set<string> uniquePatterns = set<string>(actualPatterns.begin(), actualPatterns.end());
    for (auto &p : testPatterns) {
        uniquePatterns.erase(p);
    }

    REQUIRE(uniquePatterns.empty());
}

TEST_CASE("PatternTable: assignMatchesPattern (with insertPatternAssign)") {
    Statement *stmt = TestPatterns::createStatement1();
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    vector<string> testPatterns = TestPatterns::createPatterns1();

    PatternTable table;
    table.insertPatternAssign(stmt);

    REQUIRE(table.assignMatchesPattern(stmtIndex, varName, "_"));

    for (auto pattern : testPatterns) {
        REQUIRE(table.assignMatchesPattern(stmtIndex, varName, pattern));
        REQUIRE(table.assignMatchesPattern(stmtIndex, "_", pattern));
    }

    delete stmt;
}

TEST_CASE("PatternTable: assignMatchesExactPattern (with insertPatternAssign)") {
    Statement *stmt = TestPatterns::createStatement1();
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    string testExactPattern = TestPatterns::createExactPattern1();

    PatternTable table;
    table.insertPatternAssign(stmt);

    REQUIRE(table.assignMatchesExactPattern(stmtIndex, varName, "_"));
    REQUIRE(table.assignMatchesExactPattern(stmtIndex, varName, testExactPattern));
    REQUIRE(table.assignMatchesExactPattern(stmtIndex, "_", testExactPattern));

    delete stmt;
}

TEST_CASE("PatternTable: getAssignsMatchingPattern (with insertPatternAssign)") {
    Statement *stmt = TestPatterns::createStatement1();
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    vector<string> testPatterns = TestPatterns::createPatterns1();

    PatternTable table;
    table.insertPatternAssign(stmt);

    set<StmtIndex> assignsMatchingPattern;

    assignsMatchingPattern = table.getAssignsMatchingPattern(varName, "_");
    REQUIRE(assignsMatchingPattern.size() == 1);
    REQUIRE(assignsMatchingPattern.count(stmtIndex) == 1);

    for (auto pattern : testPatterns) {
        assignsMatchingPattern = table.getAssignsMatchingPattern(varName, pattern);
        REQUIRE(assignsMatchingPattern.size() == 1);
        REQUIRE(assignsMatchingPattern.count(stmtIndex) == 1);

        assignsMatchingPattern = table.getAssignsMatchingPattern("_", pattern);
        REQUIRE(assignsMatchingPattern.size() == 1);
        REQUIRE(assignsMatchingPattern.count(stmtIndex) == 1);
    }

    delete stmt;
}

TEST_CASE("PatternTable: getAssignsMatchingExactPattern (with insertPatternAssign)") {
    Statement *stmt = TestPatterns::createStatement1();
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    string testPattern = TestPatterns::createExactPattern1();

    PatternTable table;
    table.insertPatternAssign(stmt);

    set<StmtIndex> assignsMatchingPattern;

    assignsMatchingPattern = table.getAssignsMatchingExactPattern(varName, "_");
    REQUIRE(assignsMatchingPattern.size() == 1);
    REQUIRE(assignsMatchingPattern.count(stmtIndex) == 1);

    assignsMatchingPattern = table.getAssignsMatchingExactPattern(varName, testPattern);
    REQUIRE(assignsMatchingPattern.size() == 1);
    REQUIRE(assignsMatchingPattern.count(stmtIndex) == 1);

    assignsMatchingPattern = table.getAssignsMatchingExactPattern("_", testPattern);
    REQUIRE(assignsMatchingPattern.size() == 1);
    REQUIRE(assignsMatchingPattern.count(stmtIndex) == 1);

    delete stmt;
}
