#include <set>
#include <string>
#include <vector>

#include "pkb/PatternTable.h"

#include "catch.hpp"

using namespace std;

struct TestPatterns {
    inline static VarName WILDCARD_VARNAME = "_";
    inline static ExpressionList WILDCARD_EXPRLIST = vector<string>();

    inline static string EXPRESSION_1 = "( ( x + 10 ) * ( y - z ) / 5 )";
    inline static string EXPRESSION_2 = "v + x * y + z * t";
    inline static string EXPRESSION_3 = "( a + ( b - ( c * ( d / ( e % ( f ) ) ) ) ) )";
    inline static string EXPRESSION_4 = "( ( ( ( ( ( a ) % b ) * c ) / d ) + e ) - f )";

    inline static string POSTFIX_1 = "x 10+ y z-* 5/";
    inline static string POSTFIX_2 = "v x y*+ z t*+";
    inline static string POSTFIX_3 = "a b c d e f%/*-+";
    inline static string POSTFIX_4 = "a b% c* d/ e+ f-";

    inline static vector<string> createPatterns1() {
        // INFIX: ( ( x + 10 ) * ( y - z ) / 5 )
        // POSTFIX: "x 10+ y z-* 5/"
        return {
            "x",
            "10",
            "x 10+",
            "y",
            "z",
            "y z-",
            "x 10+ y z-*",
            "5",
            "x 10+ y z-* 5/",
        };
    }

    inline static vector<string> createPatterns2() {
        // INFIX: v + x * y + z * t
        // POSTFIX: v x y*+ z t*+
        return {
            "v",
            "x",
            "y",
            "x y*",
            "v x y*+",
            "z",
            "t",
            "z t*",
            "v x y*+ z t*+",
        };
    }

    inline static vector<string> createPatterns3() {
        // INFIX: ( a + ( b - ( c * ( d / ( e % ( f ) ) ) ) ) )
        // POSTFIX: a b c d e f%/*-+
        return {
            "a",
            "b",
            "c",
            "d",
            "e",
            "f",
            "e f%",
            "d e f%/",
            "c d e f%/*",
            "b c d e f%/*-",
            "a b c d e f%/*-+",
            };
    }

    inline static vector<string> createPatterns4() {
        // INFIX: ( ( ( ( ( ( a ) % b ) * c ) / d ) + e ) - f )
        // POSTFIX: a b% c* d/ e+ f-
        return {
            "a",
            "b",
            "c",
            "d",
            "e",
            "f",
            "a b%",
            "a b% c*",
            "a b% c* d/",
            "a b% c* d/ e+",
            "a b% c* d/ e+ f-",
            };
    }

    inline static vector<string> createQueries1() {
        // INFIX: ( ( x + 10 ) * ( y - z ) / 5 )
        // POSTFIX: "x 10+ y z-* 5/"
        return {
            "x",
            "10",
            "x + 10",
            "y",
            "z",
            "y - z",
            "( x + 10 ) * ( y - z )",
            "5",
            "( x + 10 ) * ( y - z ) / 5",
            "( x )",
            "( 10 )",
            "( x + 10 )",
            "( y )",
            "( z )",
            "( y - z )",
            "( ( x + 10 ) * ( y - z ) )",
            "( 5 )",
            "( ( x + 10 ) * ( y - z ) / 5 )",
            "( x ) + 10",
            "x + ( 10 )",
            "( y ) - z",
            "y - ( z )",
            "( ( x ) + 10 ) * ( y - ( z ) )",
            "( x + ( 10 ) ) * ( ( y ) - z )",
            "( ( x + 10 ) ) * ( y - z )",
            "( x + 10 ) * ( ( y - z ) )",
            "( ( x + 10 ) ) * ( ( y - z ) )",
            "( ( x + 10 ) ) * ( y - z ) / 5",
            "( x + 10 ) * ( ( y - z ) ) / 5",
            "( ( x + 10 ) ) * ( ( y - z ) ) / 5",
            };
    }

    inline static Statement *createStatement1() {
        auto *stmt = new Statement(1, StatementType::ASSIGN);
        auto *var = new Variable("x");
        stmt->setVariable(var);
        stmt->setExpressionLst(TestPatterns::tokenizePattern(EXPRESSION_1));
        return stmt;
    }

    inline static Statement *createStatement2() {
        auto *stmt = new Statement(2, StatementType::ASSIGN);
        auto *var = new Variable("small");
        stmt->setVariable(var);
        stmt->setExpressionLst(TestPatterns::tokenizePattern(EXPRESSION_2));
        return stmt;
    }

    inline static Statement *createStatement3() {
        auto *stmt = new Statement(3, StatementType::ASSIGN);
        auto *var = new Variable("superlongasdfghjklasdfghjkl");
        stmt->setVariable(var);
        stmt->setExpressionLst(TestPatterns::tokenizePattern(EXPRESSION_3));
        return stmt;
    }

    inline static Statement *createStatement4() {
        auto *stmt = new Statement(4, StatementType::ASSIGN);
        auto *var = new Variable("withCAPSaaaADnklasdhilkAKLKDALKnasfnlK");
        stmt->setVariable(var);
        stmt->setExpressionLst(TestPatterns::tokenizePattern(EXPRESSION_4));
        return stmt;
    }

    // Example input: "((x + 10) * (y - z) / 5)"
    // Example input: ""
    inline static vector<string> tokenizePattern(Pattern &pattern) {
        vector<string> infix;
        size_t start = 0;
        size_t end = pattern.find(' ', start);

        while (end != string::npos) {
            infix.push_back(pattern.substr(start, end - start));
            start = end + 1;
            end = pattern.find(' ', start);
        }
        if (end - start > 0) {
            infix.push_back(pattern.substr(start, end - start));
        }
        return infix;
    }

    inline static bool equalPatterns(vector<string> actual, vector<string> expected) {
        if (actual.size() != expected.size()) {
            return false;
        }
        set<string> matches(actual.begin(), actual.end());
        for (auto &s : expected) {
            auto keyValuePair = matches.find(s);
            if (keyValuePair == matches.end()) {
                return false;
            }
        }
        return true;
    }
};

TEST_CASE("PatternTable: postfix patterns algorithm") {
    auto exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_1);
    auto postfix = PatternTable::createPostfix(exprList);
    auto patterns = PatternTable::createPatterns(postfix);
    REQUIRE(TestPatterns::equalPatterns(patterns, TestPatterns::createPatterns1()));

    exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_2);
    postfix = PatternTable::createPostfix(exprList);
    patterns = PatternTable::createPatterns(postfix);
    REQUIRE(TestPatterns::equalPatterns(patterns, TestPatterns::createPatterns2()));

    exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_3);
    postfix = PatternTable::createPostfix(exprList);
    patterns = PatternTable::createPatterns(postfix);
    REQUIRE(TestPatterns::equalPatterns(patterns, TestPatterns::createPatterns3()));

    exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_4);
    postfix = PatternTable::createPostfix(exprList);
    patterns = PatternTable::createPatterns(postfix);
    REQUIRE(TestPatterns::equalPatterns(patterns, TestPatterns::createPatterns4()));
}

TEST_CASE("PatternTable: postfix full pattern algorithm") {
    auto exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_1);
    auto postfix = PatternTable::createPostfix(exprList);
    auto pattern = PatternTable::createPattern(postfix);
    REQUIRE(pattern == TestPatterns::POSTFIX_1);

    exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_2);
    postfix = PatternTable::createPostfix(exprList);
    pattern = PatternTable::createPattern(postfix);
    REQUIRE(pattern == TestPatterns::POSTFIX_2);

    exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_3);
    postfix = PatternTable::createPostfix(exprList);
    pattern = PatternTable::createPattern(postfix);
    REQUIRE(pattern == TestPatterns::POSTFIX_3);

    exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_4);
    postfix = PatternTable::createPostfix(exprList);
    pattern = PatternTable::createPattern(postfix);
    REQUIRE(pattern == TestPatterns::POSTFIX_4);
}

TEST_CASE("PatternTable: assignPattern (with insertPatternAssign)") {
    Statement *stmt = TestPatterns::createStatement1();
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();

    PatternTable table;
    table.insertPatternAssign(stmt);

    REQUIRE(table.assignPattern(stmtIndex, varName, TestPatterns::WILDCARD_EXPRLIST));

    vector<string> testQueries = TestPatterns::createQueries1();
    for (auto query : testQueries) {
        auto exprList = TestPatterns::tokenizePattern(query);
        REQUIRE(table.assignPattern(stmtIndex, varName, exprList));
        REQUIRE(table.assignPattern(stmtIndex, TestPatterns::WILDCARD_VARNAME, exprList));
    }

    delete stmt;
}

TEST_CASE("PatternTable: fullAssignPattern (with insertPatternAssign)") {
    Statement *stmt = TestPatterns::createStatement2();
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();

    PatternTable table;
    table.insertPatternAssign(stmt);

    auto exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_2);
    REQUIRE(table.fullAssignPattern(stmtIndex, varName, TestPatterns::WILDCARD_EXPRLIST));
    REQUIRE(table.fullAssignPattern(stmtIndex, varName, exprList));
    REQUIRE(table.fullAssignPattern(stmtIndex, TestPatterns::WILDCARD_VARNAME, exprList));

    delete stmt;
}

TEST_CASE("PatternTable: getAssignPatternStmts (with insertPatternAssign)") {
    Statement *stmt = TestPatterns::createStatement1();
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    vector<string> testPatterns = TestPatterns::createPatterns1();

    PatternTable table;
    table.insertPatternAssign(stmt);

    set<StmtIndex> assignPatternStmts;

    assignPatternStmts = table.getAssignPatternStmts(varName, TestPatterns::WILDCARD_EXPRLIST);
    REQUIRE(assignPatternStmts.size() == 1);
    REQUIRE(assignPatternStmts.count(stmtIndex) == 1);

    vector<string> testQueries = TestPatterns::createQueries1();
    for (auto query : testQueries) {
        auto exprList = TestPatterns::tokenizePattern(query);

        assignPatternStmts = table.getAssignPatternStmts(varName, exprList);
        REQUIRE(assignPatternStmts.size() == 1);
        REQUIRE(assignPatternStmts.count(stmtIndex) == 1);

        assignPatternStmts = table.getAssignPatternStmts(TestPatterns::WILDCARD_VARNAME, exprList);
        REQUIRE(assignPatternStmts.size() == 1);
        REQUIRE(assignPatternStmts.count(stmtIndex) == 1);
    }

    delete stmt;
}

TEST_CASE("PatternTable: getFullAssignPatternStmts (with insertPatternAssign)") {
    Statement *stmt = TestPatterns::createStatement1();
    StmtIndex stmtIndex = stmt->getIndex();
    VarName varName = stmt->getVariable()->getName();
    vector<string> testPatterns = TestPatterns::createPatterns1();

    PatternTable table;
    table.insertPatternAssign(stmt);

    set<StmtIndex> fullAssignPatternStmts;

    fullAssignPatternStmts = table.getFullAssignPatternStmts(varName, TestPatterns::WILDCARD_EXPRLIST);
    REQUIRE(fullAssignPatternStmts.size() == 1);
    REQUIRE(fullAssignPatternStmts.count(stmtIndex) == 1);

    auto exprList = TestPatterns::tokenizePattern(TestPatterns::EXPRESSION_1);

    fullAssignPatternStmts = table.getFullAssignPatternStmts(varName, exprList);
    REQUIRE(fullAssignPatternStmts.size() == 1);
    REQUIRE(fullAssignPatternStmts.count(stmtIndex) == 1);

    fullAssignPatternStmts = table.getFullAssignPatternStmts(TestPatterns::WILDCARD_VARNAME, exprList);
    REQUIRE(fullAssignPatternStmts.size() == 1);
    REQUIRE(fullAssignPatternStmts.count(stmtIndex) == 1);

    delete stmt;
}
