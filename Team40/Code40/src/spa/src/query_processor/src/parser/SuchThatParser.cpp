#include "query_processor/parser/SuchThatParser.h"

void SuchThatParser::clear() {
    this->declList.clear();
    this->type = "";
    this->ref1 = "";
    this->ref2 = "";
}

Clause *SuchThatParser::parseSt(ClsTuple clsTuple) {
    type = get<0>(clsTuple);
    ref1 = get<1>(clsTuple);
    ref2 = get<2>(clsTuple);

    // choose a parser
    ClauseType clsType = clsHelper.valueToClsType(type);
    bool isStmtStmt =
        clsHelper.chooseDeType2(clsType) == DesignEntityType::STMT;
    bool isProcProc =
        clsType == ClauseType::CALLS || clsType == ClauseType::CALLS_T;
    if (isStmtStmt) {
        return parseStmtStmt();
    } else if (isProcProc) {
        return parseProcProc();
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

    ClauseType clsType = clsHelper.valueToClsType(this->type);

    Reference *r1 = parseStmtRef(this->ref1);
    Reference *r2 = parseStmtRef(this->ref2);
    if (r1 == nullptr || r2 == nullptr) {
        delete r1, r2;
        throw ValidityError("invalid clause argument");
    }

    return new Clause(clsType, *r1, *r2);
}

/**
 * Parses a clause that takes in (procedure, procedure) as parameters.
 * This includes: Calls(*).
 */
Clause *SuchThatParser::parseProcProc() {
    // quoted/synonym/wildcard, quoted/synonym/wildcard

    // procedure is a constant(quoted)/wildcard
    if (ParserUtil::isInteger(this->ref1) ||
        ParserUtil::isInteger(this->ref2)) {
        throw ValidityError("invalid clause argument");
    }

    ClauseType clsType = clsHelper.valueToClsType(this->type);

    Reference *r1 = parseEntRef(this->ref1, DesignEntityType::PROCEDURE);
    Reference *r2 = parseEntRef(this->ref2, DesignEntityType::PROCEDURE);
    if (r1 == nullptr || r2 == nullptr) {
        delete r1, r2;
        throw ValidityError("invalid clause argument");
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

    if (ParserUtil::isInteger(this->ref2)) {
        throw ValidityError("second argument cannot be integer");
    }

    Reference *r1 = getReferenceIfDeclared(this->ref1);

    // a StmtEnt is ModifiesS/UsesS
    // a EntEnt is ModifiesP/UsesP
    bool isStmtEnt = true; // false for EntEnt

    if (r1 != nullptr) {
        DesignEntityType deType = r1->getDeType();
        ReferenceType refType = r1->getRefType();
        ReferenceAttribute attr = r1->getAttr();
        if (deHelper.isStatement(deType)) {
            isStmtEnt = true;
        } else if (deHelper.isProcedure(deType)) {
            isStmtEnt = false;
        } else {
            throw ValidityError("invalid clause argument");
        }
        if (deType == DesignEntityType::PROG_LINE) {
            deType = DesignEntityType::STMT;
        }
        r1 = new Reference(deType, refType, this->ref1, attr);
    } else {
        // first argument is not declared, must be either integer or quoted
        DesignEntityType deType;
        ReferenceAttribute attr;
        if (ParserUtil::isInteger(this->ref1)) {
            isStmtEnt = true;
            deType = DesignEntityType::STMT;
            attr = ReferenceAttribute::INTEGER;
        } else if (ParserUtil::isQuoted(this->ref1)) {
            isStmtEnt = false;
            deType = DesignEntityType::PROCEDURE;
            attr = ReferenceAttribute::NAME;
            this->ref1 = ref1.substr(1, ref1.size() - 2); // remove quotes
        } else {
            throw ValidityError("invalid clause argument");
        }
        ReferenceType refT = ReferenceType::CONSTANT;
        r1 = new Reference(deType, refT, this->ref1, attr);
    }

    // second argument must always be a variable
    Reference *r2 = parseEntRef(this->ref2, DesignEntityType::VARIABLE);
    if (r2 == nullptr) {
        delete r1, r2;
        throw ValidityError("invalid clause argument");
    }

    if (isStmtEnt) {
        return new Clause(clsHelper.valueToClsType(this->type), *r1, *r2);
    }
    return new Clause(clsHelper.valueToClsType(this->type + "*"), *r1, *r2);
}
