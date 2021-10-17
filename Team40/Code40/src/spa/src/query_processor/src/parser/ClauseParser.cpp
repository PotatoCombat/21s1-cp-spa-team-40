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
    auto it = find_if(declList.begin(), declList.end(), [&syn](Reference *ref) {
        return ref->getValue() == syn;
    });

    if (it != declList.end()) {
        return *it;
    }
    return nullptr;
}

/**
 * Parses a stmt string into a `Reference` object.
 * This includes: stmt, assign, if, while, read, print, call, prog_line.
 * @param syn The name synonym.
 * @return Reference object.
 */
Reference *ClauseParser::parseStmt(string syn) {
    // syn:  SYNONYM | INTEGER | WILDCARD

    if (ParserUtil::isQuoted(syn)) {
        throw ValidityError("invalid clause argument");
    }

    Reference *r = getReferenceIfDeclared(syn);
    if (r != nullptr) {
        DesignEntityType deType = r->getDeType();
        ReferenceType refType = r->getRefType();
        ReferenceAttribute attr = r->getAttr();
        if (!deHelper.isStatement(deType)) {
            throw ValidityError("invalid clause argument");
        }
        if (deType == DesignEntityType::PROG_LINE) {
            deType = DesignEntityType::STMT;
        }
        return new Reference(deType, refType, syn, attr);
    }

    DesignEntityType deType = DesignEntityType::STMT;
    ReferenceType refType = ParserUtil::checkRefType(syn);
    ReferenceAttribute attr = ReferenceAttribute::INTEGER;
    return new Reference(deType, refType, syn, attr);
}

/**
 * Parses a variable string into a `Reference` object.
 * @param syn The variable synonym.
 * @return Reference object.
 */
Reference *ClauseParser::parseVariable(string syn) {
    // syn:  SYNONYM | QUOTED | WILDCARD

    Reference *r = getReferenceIfDeclared(syn);

    if (r != nullptr) {
        if (r->getDeType() != DesignEntityType::VARIABLE) {
            throw ValidityError("invalid reference");
        }
        return r->copy();
    }

    DesignEntityType deType = DesignEntityType::VARIABLE;
    ReferenceAttribute attr = ReferenceAttribute::NAME;
    ReferenceType refType;
    if (ParserUtil::isWildcard(syn)) {
        refType = ReferenceType::WILDCARD;
    } else if (ParserUtil::isQuoted(syn)) {
        refType = ReferenceType::CONSTANT;
        syn = syn.substr(1, syn.size() - 2);
    } else {
        throw ValidityError("invalid reference");
    }
    return new Reference(deType, refType, syn, attr);
}
