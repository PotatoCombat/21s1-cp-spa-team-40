#include "query_processor/parser/WithParser.h"

WithParser::WithParser() {
    this->ref1 = "";
    this->ref2 = "";
}

void WithParser::initReferences(vector<Reference *> &declList) {
    this->declList = declList;
}

void WithParser::clear() {
    this->declList.clear();
    this->ref1 = "";
    this->ref2 = "";
}

/**
 * Parse a WithPair into a Clause object.
 * @param withPair Pair of <ref, ref>.
 * @return Clause object of type WITH.
 */
Clause *WithParser::parse(WithPair withPair) {
    this->ref1 = withPair.first;
    this->ref2 = withPair.second;

    // the two refs must be of the same type (both strings or both integers)
    // synonym must be of type prog_line

    Reference *r1 = parseReference(ref1);
    Reference *r2 = parseReference(ref2);
    if (r1->getAttr() == r2->getAttr()) {
        return new Clause(ClauseType::WITH, *r1, *r2);
    } else {
        delete r1, r2;
        throw ValidityError("invalid with clause");
    }
}

Reference *WithParser::parseReference(string ref) {
    // ref: '"' IDENT '"' | INTEGER | attrRef | synonym
    string syn = ref;
    string attrStr = "";
    bool isAttrRef = ParserUtil::isAttrRef(ref);

    if (isAttrRef) {
        pair<string, string> attrRef = ParserUtil::splitAttrRef(ref);
        syn = attrRef.first;
        attrStr = attrRef.second;
    }

    Reference *r = getReferenceIfDeclared(syn);
    if (r != nullptr) {
        DesignEntityType deType = r->getDeType();
        ReferenceType refType = r->getRefType();
        ReferenceAttribute attr = r->getAttr();
        if (!isAttrRef && deType != DesignEntityType::PROG_LINE) {
            throw ValidityError("invalid with argument");
        }
        if (isAttrRef) {
            attr = ParserUtil::parseValidAttr(deType, attrStr);
        }
        return new Reference(deType, refType, syn, attr);
    }

    DesignEntityType deType;
    ReferenceAttribute attr;
    if (ParserUtil::isQuoted(ref)) {
        syn = syn.substr(1, syn.size() - 2);
        deType = DesignEntityType::VARIABLE;
        attr = ReferenceAttribute::NAME;
    } else if (ParserUtil::isInteger(ref)) {
        deType = DesignEntityType::STMT;
        attr = ReferenceAttribute::INTEGER;
    } else {
        throw ValidityError("invalid with argument");
    }
    return new Reference(deType, ReferenceType::CONSTANT, syn, attr);
}

/**
 * Retrieves synonym in the declaration list if it exists.
 * @param syn The synonym name.
 * @return Reference object if match, otherwise nullptr.
 */
Reference *WithParser::getReferenceIfDeclared(string syn) {
    auto it = find_if(declList.begin(), declList.end(), [&syn](Reference *ref) {
        return ref->getValue() == syn;
    });

    if (it != declList.end()) {
        return *it;
    }
    return nullptr;
}
