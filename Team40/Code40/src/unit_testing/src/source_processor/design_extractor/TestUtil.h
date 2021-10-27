#include "common/model/Statement.h"

struct TestUtil {
    inline static VarName VAR_NAME = "VAR";
    inline static ConstName CONST_NAME = "0";
    inline static vector<string> COND_EXPR =
        vector<string>{VAR_NAME, ">", CONST_NAME};

    inline static Variable variable = Variable(TestUtil::VAR_NAME);
    inline static ConstantValue constantValue =
        ConstantValue(TestUtil::CONST_NAME);

    inline static void addConditionalExpression(Statement *statement) {
        statement->setExpressionLst(COND_EXPR);
        statement->addExpressionVar(&variable);
        statement->addExpressionConst(&constantValue);
    }
};