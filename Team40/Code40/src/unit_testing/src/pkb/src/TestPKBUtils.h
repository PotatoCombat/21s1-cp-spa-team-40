#include "catch.hpp"
#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "common/model/Abstractions.h"
#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Statement.h"
#include "common/model/Variable.h"
using namespace std;

struct TestPKBUtils {
public:
    // Example input: "((x + 10) * (y - z) / 5)"
    // Example input: ""
    inline static vector<string> tokenizePattern(const string &pattern) {
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

    // Underscore == Wildcard
    inline static string WILDCARD_VARNAME = "_";

    // Empty List
    inline static vector<string> WILDCARD_EXPRLIST;

    // Standard
    inline static string EXPRESSION_1 = "v + x * y + z * t";
    inline static vector<string> INFIX_1 = tokenizePattern(EXPRESSION_1);
    inline static string EXACT_1 = "v x y*+ z t*+";
    inline static set<string> PARTIAL_1 = {
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

    // Prioritise brackets
    inline static string EXPRESSION_2 = "( ( x + 10 ) * ( y - z ) / 5 )";
    inline static vector<string> INFIX_2 = tokenizePattern(EXPRESSION_2);
    inline static string EXACT_2 = "x 10+ y z-* 5/";
    inline static set<string> PARTIAL_2 = {
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

    // Nested from right
    inline static string EXPRESSION_3 = "( a + ( b - ( c * ( d / ( e % ( f ) ) ) ) ) )";
    inline static vector<string> INFIX_3 = tokenizePattern(EXPRESSION_3);
    inline static string EXACT_3 = "a b c d e f%/*-+";
    inline static set<string> PARTIAL_3 = {
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

    // Nested from left
    inline static string EXPRESSION_4 = "( ( ( ( ( ( a ) % b ) * c ) / d ) + e ) - f )";
    inline static vector<string> INFIX_4 = tokenizePattern(EXPRESSION_4);
    inline static string EXACT_4 = "a b% c* d/ e+ f-";
    inline static set<string> PARTIAL_4 = {
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

    // INFIX: ( ( x + 10 ) * ( y - z ) / 5 )
    // POSTFIX: "x 10+ y z-* 5/"
    inline static vector<string> QUERIES_2 {
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

    template<typename T>
    inline static bool equalSets(const set<T> &actual,
                                 const set<T> &expected) {
        if (actual.size() != expected.size()) {
            return false;
        }

        size_t count = 0;
        for (auto &s : expected) {
            auto key = actual.find(s);
            if (key == actual.end()) {
                return false;
            }
            count++;
        }

        return count == expected.size();
    }

    inline static Statement createPrintStmt(int index, VarName varName) {
        Statement stmt(index, StatementType::PRINT);
        auto *variable = new Variable(varName);
        stmt.setVariable(variable);
        return stmt;
    };

    inline static Statement createReadStmt(int index, VarName varName) {
        Statement stmt(index, StatementType::READ);
        auto *variable = new Variable(varName);
        stmt.setVariable(variable);
        return stmt;
    };

    inline static Statement createCallStmt(int index, ProcName procName) {
        Statement stmt(index, StatementType::CALL);
        stmt.setProcName(procName);
        return stmt;
    };

    inline static Statement createAssignStmt(int index, VarName varName, string expressionLst) {
        Statement stmt(index, StatementType::ASSIGN);
        auto *variable = new Variable(varName);
        stmt.setVariable(variable);
        initExpressionLst(stmt, expressionLst);
        return stmt;
    };

    inline static Statement createIfStmt(int index, string conditionList) {
        Statement stmt(index, StatementType::IF);
        initExpressionLst(stmt, conditionList);
        return stmt;
    }

    inline static Statement createWhileStmt(int index, string conditionList) {
        Statement stmt(index, StatementType::WHILE);
        initExpressionLst(stmt, conditionList);
        return stmt;
    }

    inline static vector<string> createExpressionLst(const string& expression) {
        auto iss = istringstream{expression};
        return vector<string>{
            istream_iterator<string>{iss},
            istream_iterator<string>{},
            };
    }

    inline static void initExpressionLst(Statement &stmt, string expressionLst) {
        auto exprLst = createExpressionLst(expressionLst);
        stmt.setExpressionLst(exprLst);
        for (auto expressionVar : createExpressionVars(exprLst)) {
            stmt.addExpressionVar(expressionVar);
        }
        for (auto expressionConst : createExpressionConsts(exprLst)) {
            stmt.addExpressionConst(expressionConst);
        }
    }

private:

    inline static vector<Variable *> createExpressionVars(vector<string> expressionLst) {
        vector<Variable *> expressionVars;
        for (auto varName : expressionLst) {
            if (!isalpha(varName.at(0))) {
                continue;
            }
            auto *variable = new Variable(varName);
            expressionVars.push_back(variable);
        }
        return expressionVars;
    }

    inline static vector<ConstantValue *> createExpressionConsts(vector<string> expressionLst) {
        vector<ConstantValue *> expressionConsts;
        for (auto constName : expressionLst) {
            if (!isdigit(constName.at(0))) {
                continue;
            }
            auto *constant = new ConstantValue(constName);
            expressionConsts.push_back(constant);
        }
        return expressionConsts;
    }
};
