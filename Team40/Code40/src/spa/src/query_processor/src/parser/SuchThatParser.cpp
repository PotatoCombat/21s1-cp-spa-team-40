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
    this->declList.clear();
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

    // statement is a constant(integer)/wildcard
    if (ParserUtil::isQuoted(this->ref1) || ParserUtil::isQuoted(this->ref2)) {
        throw ValidityError("invalid clause argument");
    }

    Reference *r1 = getReferenceIfDeclared(this->ref1);
    Reference *r2 = getReferenceIfDeclared(this->ref2);

    ClauseType clsType = clsHelper.valueToClsType(this->type);

    if (r1 != nullptr) {
        if (!deHelper.isStatement(r1->getDeType())) {
            throw ValidityError("invalid clause argument");
        }
        r1 = r1->copy();
    } else {
        DesignEntityType deType1 = DesignEntityType::STMT;
        ReferenceType refT = ParserUtil::checkRefType(this->ref1);
        r1 = new Reference(deType1, refT, this->ref1);
    }

    if (r2 != nullptr) {
        if (!deHelper.isStatement(r2->getDeType())) {
            delete r1;
            throw ValidityError("invalid clause argument");
        }
        r2 = r2->copy();
    } else {
        DesignEntityType deType2 = DesignEntityType::STMT;
        ReferenceType refT = ParserUtil::checkRefType(this->ref2);
        r2 = new Reference(deType2, refT, this->ref2);
    }

    return new Clause(clsType, *r1, *r2);
}

/**
 * Parses a clause that takes in (X, ent) as parameters. 
 * This includes: Modifies/Uses (both P and S versions).
 */
Clause *SuchThatParser::parseXEnt() {
    // MODIFIES_S/USES_S: number/synonym, quoted/synonym/wildcard
    // MODIFIES_P/USES_P: quoted/synonym, quoted/synonym/wildcard

    if (ParserUtil::isWildcard(this->ref1)) {
        throw ValidityError("first argument cannot be wildcard");
    }

    Reference *r1 = getReferenceIfDeclared(this->ref1);
    Reference *r2 = getReferenceIfDeclared(this->ref2);

    // a StmtEnt is ModifiesS/UsesS
    // a EntEnt is ModifiesP/UsesP
    bool isStmtEnt = true; // false for EntEnt

    if (r1 != nullptr) {
        if (deHelper.isStatement(r1->getDeType())) {
            isStmtEnt = true;
        } else if (deHelper.isProcedure(r1->getDeType())) {
            isStmtEnt = false;
        } else {
            throw ValidityError("invalid clause argument");
        }
        r1 = r1->copy();
    } else {
        // first argument is not declared, must be either integer or quoted
        DesignEntityType deType1;
        if (ParserUtil::isInteger(this->ref1)) {
            isStmtEnt = true;
            deType1 = DesignEntityType::STMT;
        } else if (ParserUtil::isQuoted(this->ref1)) {
            isStmtEnt = false;
            deType1 = DesignEntityType::PROCEDURE;
            this->ref1 = ref1.substr(1, ref1.size() - 2); // remove quotes
        } else {
            throw ValidityError("invalid clause argument");
        }
        ReferenceType refT = ReferenceType::CONSTANT;
        r1 = new Reference(deType1, refT, this->ref1);
    }

    // second argument must always be a variable
    if (r2 != nullptr) {
        if (!deHelper.isVariable(r2->getDeType())) {
            delete r1;
            throw ValidityError("invalid clause argument");
        }
        r2 = r2->copy();
    } else {
        DesignEntityType deType2 = DesignEntityType::VARIABLE;
        ReferenceType refT = ParserUtil::checkRefType(this->ref2);
        if (refT == ReferenceType::CONSTANT) { // remove quotes if constant
            ref2 = ref2.substr(1, ref2.size() - 2);
        }
        r2 = new Reference(deType2, refT, this->ref2);
    }

    if (isStmtEnt) {
        return new Clause(clsHelper.valueToClsType(this->type), *r1, *r2);
    }
    return new Clause(clsHelper.valueToClsType(this->type + "*"), *r1, *r2);
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
