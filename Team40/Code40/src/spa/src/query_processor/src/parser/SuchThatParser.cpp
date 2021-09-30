#include "query_processor/parser/SuchThatParser.h"

SuchThatParser::SuchThatParser() {
    this->type = "";
    this->ref1 = "";
    this->ref2 = "";
}

void SuchThatParser::initReferences(vector<Reference *> &declList) {
    this->declList = declList;
}

void SuchThatParser::clear() {
    this->declList = vector<Reference *>();
    this->type = "";
    this->ref1 = "";
    this->ref2 = "";
}

Clause *SuchThatParser::parse(ClsTuple clsTuple) {
    type = get<0>(clsTuple);
    ref1 = get<1>(clsTuple);
    ref2 = get<2>(clsTuple);

    // choose a parser
    ClauseType clsType = clsHelper.valueToClsType(type);
    bool isStmtStmt =
        clsHelper.chooseDeType2(clsType) == DesignEntityType::STMT;
    if (isStmtStmt) {
        return parseStmtStmt();
    } else {
        return parseXEnt();
    }
}

/**
 * Parses a clause that takes in (stmt, stmt) as parameters. 
 * This includes: Follows(*)/Parent(*)/Next(*)/Affects(*).
 */
Clause *SuchThatParser::parseStmtStmt() {
    // number/synonym/wildcard, number/synonym/wildcard
    ClauseType clsType = clsHelper.valueToClsType(this->type);
    Reference *r1 = getReferenceIfDeclared(this->ref1);
    Reference *r2 = getReferenceIfDeclared(this->ref2);

    DesignEntityType deType1 = clsHelper.chooseDeType1(clsType);
    DesignEntityType deType2 = clsHelper.chooseDeType2(clsType);

    // both are declared synonyms
    if (r1 != nullptr && r2 != nullptr) {
        // check correct entity types
        if (r1->getDeType() == deType1 && r2->getDeType() == deType2) {
            r1 = r1->copy();
            r2 = r2->copy();
        } else {
            throw ValidityError("invalid clause argument");
        }
    } else {
        // statement is a constant(integer)/wildcard
        if (ParserUtil::isQuoted(this->ref1) ||
            ParserUtil::isQuoted(this->ref2)) {
            throw ValidityError("invalid clause argument");
        }
        if (r1 != nullptr) {
            // r1 is declared synonym
            if (r1->getDeType() == deType1) {
                r1 = r1->copy();
            } else {
                throw ValidityError("invalid clause argument");
            }
            ReferenceType refT = ParserUtil::checkRefType(this->ref1);
            r1 = new Reference(deType1, refT, this->ref1);
        } else if (r2 != nullptr) {
            // r2 is declared synonym
            if (r2->getDeType() == deType2) {
                r2 = r2->copy();
            } else {
                throw ValidityError("invalid clause argument");
            }
            ReferenceType refT = ParserUtil::checkRefType(this->ref2);
            r2 = new Reference(deType2, refT, this->ref2);
        } else {
            // neither are declared
            ReferenceType refT1 = ParserUtil::checkRefType(this->ref1);
            ReferenceType refT2 = ParserUtil::checkRefType(this->ref2);
            r1 = new Reference(deType1, refT1, this->ref1);
            r2 = new Reference(deType2, refT2, this->ref2);
        }
    }

    return new Clause(clsType, *r1, *r2);
}

/**
 * Parses a clause that takes in (X, ent) as parameters.
 * This includes: Modifies/Uses (both P and S versions).
 */
Clause *SuchThatParser::parseXEnt() {
 
}

/**
 * Retrieves synonym in the declaration list if it exists.
 * @param syn The synonym name.
 * @return Reference object if match, otherwise nullptr.
 */
Reference *SuchThatParser::getReferenceIfDeclared(string syn) {
    auto it = find_if(declList.begin(), declList.end(), [&syn](Reference *ref) {
        return ref->getValue() == syn;
    });

    if (it != declList.end()) {
        return *it;
    }
    return nullptr;
}
