#include "catch.hpp"
#include "common/model/Abstractions.h"
#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Statement.h"
#include "common/model/Variable.h"
#include "pkb/PKB.h"
#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct TestIntegrationUtils {
public:
    inline static int INDEX = 1;

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

    inline static Procedure createProcedure(ProcName procName) {
        Procedure proc(procName);
        return proc;
    };

    inline static Statement *createPrintStmt(int index, VarName varName) {
        auto *stmt = new Statement(index, StatementType::PRINT);
        auto *variable = new Variable(varName);
        stmt->setVariable(variable);
        return stmt;
    };

    inline static Statement *createReadStmt(int index, VarName varName) {
        auto *stmt = new Statement(index, StatementType::READ);
        auto *variable = new Variable(varName);
        stmt->setVariable(variable);
        return stmt;
    };

    inline static Statement *createCallStmt(int index, ProcName procName) {
        auto *stmt = new Statement(index, StatementType::CALL);
        stmt->setProcName(procName);
        return stmt;
    };

    inline static Statement *createAssignStmt(int index, VarName varName, string expressionLst) {
        auto *stmt = new Statement(index, StatementType::ASSIGN);
        auto *variable = new Variable(varName);
        stmt->setVariable(variable);
        initExpressionLst(stmt, expressionLst);
        return stmt;
    };

    inline static Statement *createIfStmt(int index, string conditionList) {
        auto *stmt = new Statement(index, StatementType::IF);
        initExpressionLst(stmt, conditionList);
        return stmt;
    }

    inline static Statement *createWhileStmt(int index, string conditionList) {
        auto *stmt = new Statement(index, StatementType::WHILE);
        initExpressionLst(stmt, conditionList);
        return stmt;
    }

    inline static vector<string> createExpressionLst(const string &expression) {
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

    inline static void initExpressionLst(Statement *stmt, string expressionLst) {
        auto exprLst = createExpressionLst(expressionLst);
        stmt->setExpressionLst(exprLst);
        for (auto expressionVar : createExpressionVars(exprLst)) {
            stmt->addExpressionVar(expressionVar);
        }
        for (auto expressionConst : createExpressionConsts(exprLst)) {
            stmt->addExpressionConst(expressionConst);
        }
    }

    inline static vector<Statement *> createExampleStmts() {
        int s = 0;
        return vector<Statement *> {
            TestIntegrationUtils::createReadStmt(s++, "x"), // Dummy 0 index
            TestIntegrationUtils::createReadStmt(s++, "x"),
            TestIntegrationUtils::createAssignStmt(s++, "y", "1"),
            TestIntegrationUtils::createAssignStmt(s++, "z", "x + y"),
            TestIntegrationUtils::createWhileStmt(s++, "i > 9"),
            TestIntegrationUtils::createAssignStmt(s++, "z", "z + i"),
            TestIntegrationUtils::createAssignStmt(s++, "i", "i - 1"),
            TestIntegrationUtils::createAssignStmt(s++, "i",
                                                   "10 + y * z / x + 5 * x - 3 * y"),
            TestIntegrationUtils::createIfStmt(s++, "z == 10"),
            TestIntegrationUtils::createPrintStmt(s++, "x"),
            TestIntegrationUtils::createPrintStmt(s++, "y"),
            TestIntegrationUtils::createWhileStmt(s++, "z != 4"),
            TestIntegrationUtils::createAssignStmt(s++, "z", "z - 1"),
            TestIntegrationUtils::createPrintStmt(s++, "z"),
            TestIntegrationUtils::createIfStmt(s++, "z == 6"),
            TestIntegrationUtils::createPrintStmt(s++, "z"),
            TestIntegrationUtils::createPrintStmt(s++, "y"),
        };
    }

    inline static Procedure createExampleProcedure() {
        Procedure procedure = TestIntegrationUtils::createProcedure("Examp1le");

        int s = 0;
        vector<Statement *> stmts = createExampleStmts();

        stmts[4]->addThenStmt(stmts[5]);
        stmts[4]->addThenStmt(stmts[6]);

        stmts[8]->addThenStmt(stmts[9]);
        stmts[8]->addThenStmt(stmts[10]);
        stmts[8]->addElseStmt(stmts[11]);

        stmts[11]->addThenStmt(stmts[12]);
        stmts[11]->addThenStmt(stmts[13]);

        stmts[14]->addThenStmt(stmts[15]);
        stmts[14]->addElseStmt(stmts[16]);

        procedure.addToStmtLst(stmts[1]);
        procedure.addToStmtLst(stmts[2]);
        procedure.addToStmtLst(stmts[3]);
        procedure.addToStmtLst(stmts[4]);
        procedure.addToStmtLst(stmts[7]);
        procedure.addToStmtLst(stmts[8]);
        procedure.addToStmtLst(stmts[14]);

        return procedure;
    }

    inline static PKB createExamplePKB() {
        PKB pkb;

        vector<Statement *> stmts = TestIntegrationUtils::createExampleStmts();
        Procedure procedure = TestIntegrationUtils::createExampleProcedure();

        pkb.insertFollows(stmts[1], stmts[2]);
        pkb.insertFollows(stmts[2], stmts[3]);
        pkb.insertFollows(stmts[3], stmts[4]);
        pkb.insertFollows(stmts[4], stmts[7]);
        pkb.insertFollows(stmts[7], stmts[8]);
        pkb.insertFollows(stmts[8], stmts[14]);
        pkb.insertFollows(stmts[5], stmts[6]);
        pkb.insertFollows(stmts[9], stmts[10]);
        pkb.insertFollows(stmts[12], stmts[13]);

        pkb.insertAssignPattern(stmts[2]);
        pkb.insertAssignPattern(stmts[3]);
        pkb.insertAssignPattern(stmts[5]);
        pkb.insertAssignPattern(stmts[6]);
        pkb.insertAssignPattern(stmts[7]);
        pkb.insertAssignPattern(stmts[12]);

        pkb.insertIfPattern(stmts[8]);
        pkb.insertIfPattern(stmts[14]);

        pkb.insertWhilePattern(stmts[4]);
        pkb.insertWhilePattern(stmts[12]);

        return pkb;
    }
};
