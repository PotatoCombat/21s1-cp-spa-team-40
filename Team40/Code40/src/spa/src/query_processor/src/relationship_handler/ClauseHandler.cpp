#include "query_processor/relationship_handler/ClauseHandler.h"

StatementType ClauseHandler::desTypeToStmtType(DesignEntityType desType) {
    if (desType == DesignEntityType::PROCEDURE ||
        desType == DesignEntityType::CONSTANT ||
        desType == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("RelationshipHandler.desTypeToStmtType(): desType is not of type stmt");
    }

    switch (desType) {
    case DesignEntityType::ASSIGN:
        return StatementType::ASSIGN;
        break;
    case DesignEntityType::READ:
        return StatementType::READ;
        break;
    case DesignEntityType::PRINT:
        return StatementType::PRINT;
        break;
    case DesignEntityType::WHILE:
        return StatementType::WHILE;
        break;
    case DesignEntityType::IF:
        return StatementType::IF;
        break;
    case DesignEntityType::CALL:
        return StatementType::CALL;
        break;
    default:
        return StatementType::UNKNOWN;
        break;
    }
}

bool ClauseHandler::isDesTypeStmtType(DesignEntityType desType, StatementType stmtType) {
    if (desType == DesignEntityType::PROCEDURE || 
        desType == DesignEntityType::CONSTANT || 
        desType == DesignEntityType::VARIABLE) {
        return false;
    }

    if (desType == DesignEntityType::STMT) {
        return true;
    }
    
    return desTypeToStmtType(desType) == stmtType;
}