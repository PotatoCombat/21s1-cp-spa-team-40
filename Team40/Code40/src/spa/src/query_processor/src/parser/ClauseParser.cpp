#include "query_processor/parser/ClauseParser.h"

DesignEntityTypeHelper ClauseParser::deHelper = DesignEntityTypeHelper();
ClauseTypeHelper ClauseParser::clsHelper = ClauseTypeHelper();

void ClauseParser::initReferences(vector<Reference *> &declList) {
    this->declList = declList;
}

Clause *ClauseParser::parseSt(ClsTuple clsTuple) {
    throw runtime_error("invalid call");
}

Clause *ClauseParser::parsePt(PatTuple patTuple) {
    throw runtime_error("invalid call");
}

Clause *ClauseParser::parseWt(WithPair withPair) {
    throw runtime_error("invalid call");
}

/**
 * Retrieves synonym in the declaration list if it exists.
 * @param syn The synonym name.
 * @return Reference object if match, otherwise nullptr.
 */
Reference *ClauseParser::getReferenceIfDeclared(string syn) {
    return ParserUtil::getReferenceFromList(declList, syn);
}

/**
 * Parses a `stmtRef` string into a `Reference` object.
 * This includes: stmt, assign, if, while, read, print, call, prog_line.
 * @param syn The stmtRef.
 * @return Reference object, otherwise nullptr.
 */
Reference *ClauseParser::parseStmtRef(string syn) {
    // syn:  SYNONYM | INTEGER | WILDCARD

    if (ParserUtil::isQuoted(syn)) {
        return nullptr;
    }

    Reference *r = getReferenceIfDeclared(syn);
    if (r != nullptr) {
        DesignEntityType deType = r->getDeType();
        if (!deHelper.isStatement(deType)) {
            return nullptr;
        }
        if (deType == DesignEntityType::PROG_LINE) {
            deType = DesignEntityType::STMT;
        }
        return new Reference(deType, r->getRefType(), syn, r->getAttr());
    }

    DesignEntityType deType = DesignEntityType::STMT;
    ReferenceType refType = ParserUtil::checkRefType(syn);
    ReferenceAttribute attr = ReferenceAttribute::INTEGER;
    return new Reference(deType, refType, syn, attr);
}

/**
 * Parses a `entRef` string into a `Reference` object.
 * @param syn The entRef.
 * @return Reference object, otherwise nullptr.
 */
Reference *ClauseParser::parseEntRef(string syn, DesignEntityType type) {
    // syn:  SYNONYM | QUOTED | WILDCARD

    if (ParserUtil::isInteger(syn)) {
        return nullptr;
    }

    Reference *r = getReferenceIfDeclared(syn);
    if (r != nullptr) {
        if (r->getDeType() != type) {
            return nullptr;
        }
        return r->copy();
    }

    ReferenceAttribute attr = ReferenceAttribute::NAME;
    ReferenceType refType = ParserUtil::checkRefType(syn);
    if (ParserUtil::isQuoted(syn)) {
        syn = syn.substr(1, syn.size() - 2);
    }
    return new Reference(type, refType, syn, attr);
}
