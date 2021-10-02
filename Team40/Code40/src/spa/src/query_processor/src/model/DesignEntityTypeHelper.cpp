#include "query_processor/model/DesignEntityTypeHelper.h"

map<DesignEntityType, StatementType> DesignEntityTypeHelper::desTypeToStmtTypeMap{
    {DesignEntityType::ASSIGN, StatementType::ASSIGN},
    {DesignEntityType::CALL, StatementType::CALL},
    {DesignEntityType::IF, StatementType::IF},
    {DesignEntityType::PRINT, StatementType::PRINT},
    {DesignEntityType::READ, StatementType::READ},
    {DesignEntityType::WHILE, StatementType::WHILE}};

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
    desTypeToStmtTypeMap = {
        {DesignEntityType::ASSIGN, StatementType::ASSIGN},
        {DesignEntityType::CALL, StatementType::CALL},
        {DesignEntityType::IF, StatementType::IF},
        {DesignEntityType::PRINT, StatementType::PRINT},
        {DesignEntityType::READ, StatementType::READ},
        {DesignEntityType::WHILE, StatementType::WHILE}
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

StatementType DesignEntityTypeHelper::desTypeToStmtType(DesignEntityType type) {
    auto val = desTypeToStmtTypeMap.find(type);
    if (val == desTypeToStmtTypeMap.end()) {
        throw ValidityError("invalid design entity type is not a statement type");
    }
    return val->second;
}

bool DesignEntityTypeHelper::isDesTypeStmtType(DesignEntityType desType, StatementType stmtType) {
    return desTypeToStmtType(desType) == stmtType;
}
