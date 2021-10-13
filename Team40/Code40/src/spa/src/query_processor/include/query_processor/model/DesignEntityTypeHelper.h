#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/DesignEntityType.h"
#include "query_processor/model/Reference.h"

#include "common/model/Statement.h"

using namespace std;

class DesignEntityTypeHelper {
private:
    unordered_map<string, DesignEntityType> stringToTypeMap;
    unordered_map<DesignEntityType, bool> typeToIsStmtMap;
    unordered_map<DesignEntityType, ReferenceAttribute> typeToDefaultAttrMap;
    static map<DesignEntityType, StatementType> desTypeToStmtTypeMap;

public:
    DesignEntityTypeHelper();

    // get type from string
    DesignEntityType valueToDesType(string val);
    ReferenceAttribute typeToDefaultAttr(DesignEntityType type);

    bool isStatement(DesignEntityType type);
    bool isVariable(DesignEntityType type);
    bool isProcedure(DesignEntityType type);

    // get stmt type from design entity type
    static StatementType desTypeToStmtType(DesignEntityType type);
    static bool isDesTypeStmtType(DesignEntityType desType,
                                  StatementType stmtType);
};
