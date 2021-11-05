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

#include "catch.hpp"

using namespace std;

struct TestUtils {
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

    inline static Statement createPrintStmt(const StmtIndex &index,
                                            const VarName &varName) {
        Statement stmt(index, StatementType::PRINT);
        auto *variable = new Variable(varName);
        stmt.setVariable(variable);
        return stmt;
    };

    inline static Statement createReadStmt(const StmtIndex &index,
                                           const VarName &varName) {
        Statement stmt(index, StatementType::READ);
        auto *variable = new Variable(varName);
        stmt.setVariable(variable);
        return stmt;
    };

    inline static Statement createCallStmt(const StmtIndex &index,
                                           const ProcName &procName) {
        Statement stmt(index, StatementType::CALL);
        stmt.setProcName(procName);
        return stmt;
    };

    inline static Statement createAssignStmt(const StmtIndex &index,
                                             const VarName &varName,
                                             const string &expressionLst) {
        Statement stmt(index, StatementType::ASSIGN);
        auto *variable = new Variable(varName);
        stmt.setVariable(variable);
        initExpressionLst(stmt, expressionLst);
        return stmt;
    };

    inline static Statement createIfStmt(const StmtIndex &index,
                                         const string &conditionList) {
        Statement stmt(index, StatementType::IF);
        initExpressionLst(stmt, conditionList);
        return stmt;
    }

    inline static Statement createWhileStmt(const StmtIndex &index,
                                            const string &conditionList) {
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

private:
    inline static void initExpressionLst(Statement &stmt,
                                         const string &expressionLst) {
        auto exprLst = createExpressionLst(expressionLst);
        stmt.setExpressionLst(exprLst);
        for (auto expressionVar : createExpressionVars(exprLst)) {
            stmt.addExpressionVar(expressionVar);
        }
        for (auto expressionConst : createExpressionConsts(exprLst)) {
            stmt.addExpressionConst(expressionConst);
        }
    }

    inline static vector<Variable *> createExpressionVars(
            const ExpressionList &expressionLst) {
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

    inline static vector<ConstantValue *> createExpressionConsts(
            const ExpressionList &expressionLst) {
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
