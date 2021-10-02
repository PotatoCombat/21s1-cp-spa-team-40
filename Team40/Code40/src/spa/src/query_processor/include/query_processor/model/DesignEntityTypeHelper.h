#pragma once

#include <string>
#include <map>

#include "query_processor/model/DesignEntityType.h"
#include "query_processor/exception/ValidityError.h"

#include "common/model/Statement.h"

using namespace std;

class DesignEntityTypeHelper {
private:
    map<DesignEntityType, string> typeToStringMap;
    map<string, DesignEntityType> stringToTypeMap;
    map<DesignEntityType, bool> typeToIsStmtMap;
    static map<DesignEntityType, StatementType> desTypeToStmtTypeMap;

public:
    DesignEntityTypeHelper();

    // get type from string
    DesignEntityType getType(string val);

    // get value from type
    string getValue(DesignEntityType type);

    bool isStatement(DesignEntityType type);
    bool isVariable(DesignEntityType type);
    bool isProcedure(DesignEntityType type);

    // get stmt type from design entity type
    static StatementType desTypeToStmtType(DesignEntityType type);
    static bool isDesTypeStmtType(DesignEntityType desType, StatementType stmtType);
};
