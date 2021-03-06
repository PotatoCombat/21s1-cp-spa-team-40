#include "query_processor/parser/WithParser.h"

void WithParser::clear() {
    this->declList.clear();
    delete this->r1, this->r2;
    this->r1 = nullptr;
    this->r2 = nullptr;
    this->ref1 = "";
    this->ref2 = "";
}

/**
 * Parses a WithPair into a Clause object.
 * @param withPair Pair of <ref, ref>.
 * @return Clause object of type WITH.
 */
Clause *WithParser::parseWt(WithPair withPair) {
    this->ref1 = withPair.first;
    this->ref2 = withPair.second;

    // the two refs must be of the same type (both strings or both integers)
    // synonym must be of type prog_line

    r1 = parseReference(ref1);
    r2 = parseReference(ref2);
    if (r1->getAttr() == r2->getAttr()) {
        return new Clause(ClauseType::WITH, *r1, *r2);
    } else {
        throw ValidityError("QP-ERROR: invalid with clause");
    }
}

Reference *WithParser::parseReference(string ref) {
    // ref: '"' IDENT '"' | INTEGER | attrRef | synonym
    string syn = ref;
    string attrStr = "";
    bool isAttrRef = ParserUtil::isAttrRef(ref);

    if (isAttrRef) {
        AttrRef attrRef = ParserUtil::splitAttrRef(ref);
        syn = attrRef.first;
        attrStr = attrRef.second;
    }

    Reference *r = getReferenceIfDeclared(syn);
    if (r != nullptr) {
        DesignEntityType deType = r->getDeType();
        ReferenceType refType = r->getRefType();
        ReferenceAttribute attr = r->getAttr();
        if (!isAttrRef && deType != DesignEntityType::PROG_LINE) {
            throw ValidityError("QP-ERROR: invalid with argument");
        }
        if (isAttrRef) {
            attr = ParserUtil::parseValidAttr(deType, attrStr);
        }
        if (deType == DesignEntityType::PROG_LINE) {
            deType = DesignEntityType::STMT;
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
        throw ValidityError("QP-ERROR: invalid with argument");
    }
    return new Reference(deType, ReferenceType::CONSTANT, syn, attr);
}
