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

struct TestCommonUtils {
public:
    inline static VarName VARNAME_1 = "A";
    inline static VarName VARNAME_2 = "B";
    inline static VarName VARNAME_3 = "C";

    inline static ConstName CONSTNAME_1 = "1";
    inline static ConstName CONSTNAME_2 = "2";
    inline static ConstName CONSTNAME_3 = "3";

    inline static ProcName PROCNAME_1 = "a";
    inline static ProcName PROCNAME_2 = "b";
    inline static ProcName PROCNAME_3 = "c";

    inline static string EXPRLST_1 = "A + 1";
    inline static string EXPRLST_2 = "2 * B";
    inline static string EXPRLST_3 = "( C % 3 )";

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
};
