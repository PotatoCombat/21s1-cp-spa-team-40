#include "TestUtil.h"

void addConditionalExpression(Statement *statement) {
    VarName VAR_NAME = "VAR";
    ConstName CONST_NAME = "0";
    vector<string> COND_EXPR = vector<string>{VAR_NAME, ">", CONST_NAME};

    Variable variable(VAR_NAME);
    ConstantValue constantValue(CONST_NAME);

    statement->setExpressionLst(COND_EXPR);
    statement->addExpressionVar(&variable);
    statement->addExpressionConst(&constantValue);
}
