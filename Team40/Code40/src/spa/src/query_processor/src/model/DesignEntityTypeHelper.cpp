#include "query_processor/model/DesignEntityTypeHelper.h"

DesignEntityTypeHelper::DesignEntityTypeHelper() {
    typeToStringMap = {
        {DesignEntityType::STMT, "stmt"},
        {DesignEntityType::ASSIGN, "assign"},
        {DesignEntityType::VARIABLE, "variable"},
        {DesignEntityType::CONSTANT, "constant"},
        {DesignEntityType::PROCEDURE, "procedure"},
        {DesignEntityType::READ, "read"},
        {DesignEntityType::PRINT, "print"},
        {DesignEntityType::WHILE, "while"},
        {DesignEntityType::IF, "if"},
        {DesignEntityType::CALL, "call"},
        {DesignEntityType::PROG_LINE, "prog_line"}
    };
    stringToTypeMap = {
        {"stmt", DesignEntityType::STMT},
        {"assign", DesignEntityType::ASSIGN},
        {"variable", DesignEntityType::VARIABLE},
        {"constant", DesignEntityType::CONSTANT},
        {"procedure", DesignEntityType::PROCEDURE},
        {"read", DesignEntityType::READ},
        {"print", DesignEntityType::PRINT},
        {"while", DesignEntityType::WHILE},
        {"if", DesignEntityType::IF},
        {"call", DesignEntityType::CALL},
        {"prog_line", DesignEntityType::PROG_LINE}
    };
    typeToIsStmtMap = {
        {DesignEntityType::STMT, true},
        {DesignEntityType::ASSIGN, true},
        {DesignEntityType::VARIABLE, false},
        {DesignEntityType::CONSTANT, false},
        {DesignEntityType::PROCEDURE, false},
        {DesignEntityType::READ, true},
        {DesignEntityType::PRINT, true},
        {DesignEntityType::WHILE, true},
        {DesignEntityType::IF, true},
        {DesignEntityType::CALL, true},
        {DesignEntityType::PROG_LINE, false}
    };
}

DesignEntityType DesignEntityTypeHelper::getType(string val) {
    auto type = stringToTypeMap.find(val);
    if (type == stringToTypeMap.end()) {
        throw ValidityError("invalid design entity type");
    }
    return type->second;
}

string DesignEntityTypeHelper::getValue(DesignEntityType type) {
    auto val = typeToStringMap.find(type);
    if (val == typeToStringMap.end()) { // typically should not reach here
        throw ValidityError("invalid design entity type"); // TODO: throw runtime error instead
    }
    return val->second;
}

bool DesignEntityTypeHelper::isStatement(DesignEntityType type) {
    auto val = typeToIsStmtMap.find(type);
    if (val == typeToIsStmtMap.end()) {
        throw ValidityError("invalid design entity type"); // TODO: throw runtime error instead
    }
    return val->second;
}

bool DesignEntityTypeHelper::isVariable(DesignEntityType type) {
    return type == DesignEntityType::VARIABLE;
}

bool DesignEntityTypeHelper::isProcedure(DesignEntityType type) {
    return type == DesignEntityType::PROCEDURE;
}
