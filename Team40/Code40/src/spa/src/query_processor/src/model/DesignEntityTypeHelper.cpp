#include "query_processor/model/DesignEntityTypeHelper.h"

map<DesignEntityType, StatementType> DesignEntityTypeHelper::desTypeToStmtTypeMap{
    {DesignEntityType::ASSIGN, StatementType::ASSIGN},
    {DesignEntityType::CALL, StatementType::CALL},
    {DesignEntityType::IF, StatementType::IF},
    {DesignEntityType::PRINT, StatementType::PRINT},
    {DesignEntityType::READ, StatementType::READ},
    {DesignEntityType::WHILE, StatementType::WHILE}};

DesignEntityTypeHelper::DesignEntityTypeHelper() {
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
        {"prog_line", DesignEntityType::PROG_LINE} // TODO: replace all prog_lines with STMT in the future?
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
        {DesignEntityType::PROG_LINE, true}
    };
    typeToAttrMap = {
        {DesignEntityType::STMT, ReferenceAttribute::INTEGER},
        {DesignEntityType::ASSIGN, ReferenceAttribute::INTEGER},
        {DesignEntityType::VARIABLE, ReferenceAttribute::NAME},
        {DesignEntityType::CONSTANT, ReferenceAttribute::INTEGER},
        {DesignEntityType::PROCEDURE, ReferenceAttribute::NAME},
        {DesignEntityType::READ, ReferenceAttribute::INTEGER},
        {DesignEntityType::PRINT, ReferenceAttribute::INTEGER},
        {DesignEntityType::WHILE, ReferenceAttribute::INTEGER},
        {DesignEntityType::IF, ReferenceAttribute::INTEGER},
        {DesignEntityType::CALL, ReferenceAttribute::INTEGER},
        {DesignEntityType::PROG_LINE, ReferenceAttribute::INTEGER}
    };
    hasNonDefaultAttrMap = {
        {DesignEntityType::STMT, false},
        {DesignEntityType::ASSIGN, false},
        {DesignEntityType::VARIABLE, false},
        {DesignEntityType::CONSTANT, false},
        {DesignEntityType::PROCEDURE, false},
        {DesignEntityType::READ, true},
        {DesignEntityType::PRINT, true},
        {DesignEntityType::WHILE, false},
        {DesignEntityType::IF, false},
        {DesignEntityType::CALL, true},
        {DesignEntityType::PROG_LINE, false}
    };
}

DesignEntityType DesignEntityTypeHelper::valueToDesType(string val) {
    auto type = stringToTypeMap.find(val);
    if (type == stringToTypeMap.end()) {
        throw ValidityError("invalid design entity type");
    }
    return type->second;
}

ReferenceAttribute DesignEntityTypeHelper::typeToAttr(DesignEntityType type) {
    auto val = typeToAttrMap.find(type);
    if (val == typeToAttrMap.end()) {
        throw ValidityError("invalid design entity type");
    }
    return val->second;
}

bool DesignEntityTypeHelper::isStatement(DesignEntityType type) {
    auto val = typeToIsStmtMap.find(type);
    if (val == typeToIsStmtMap.end()) {
        throw ValidityError("invalid design entity type");
    }
    return val->second;
}

bool DesignEntityTypeHelper::isVariable(DesignEntityType type) {
    return type == DesignEntityType::VARIABLE;
}

bool DesignEntityTypeHelper::isProcedure(DesignEntityType type) {
    return type == DesignEntityType::PROCEDURE;
}

bool DesignEntityTypeHelper::hasNonDefaultAttr(DesignEntityType type) {
    auto val = hasNonDefaultAttrMap.find(type);
    if (val == hasNonDefaultAttrMap.end()) {
        throw ValidityError("invalid design entity type");
    }
    return val->second;
}

StatementType DesignEntityTypeHelper::desTypeToStmtType(DesignEntityType type) {
    auto val = desTypeToStmtTypeMap.find(type);
    if (val == desTypeToStmtTypeMap.end()) {
        throw ValidityError(
            "invalid design entity type is not a statement type");
    }
    return val->second;
}

bool DesignEntityTypeHelper::isDesTypeStmtType(DesignEntityType desType,
                                               StatementType stmtType) {
    return desTypeToStmtType(desType) == stmtType;
}
