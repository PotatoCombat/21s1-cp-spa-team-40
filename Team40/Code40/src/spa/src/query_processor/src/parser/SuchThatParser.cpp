#include "query_processor/parser/SuchThatParser.h"

void SuchThatParser::clear() {
    this->declList.clear();
    delete this->r1, this->r2;
    this->r1 = nullptr;
    this->r2 = nullptr;
    this->type = "";
    this->ref1 = "";
    this->ref2 = "";
}

/**
 * Parses a ClsTuple into a Clause object.
 * @param clsTuple Tuple of <type, ref, ref>.
 * @return Clause object of some `such that` type.
 */
Clause *SuchThatParser::parseSt(ClsTuple clsTuple) {
    type = get<0>(clsTuple);
    ref1 = get<1>(clsTuple);
    ref2 = get<2>(clsTuple);

    // choose a parser
    ClauseType clsType = clsHelper.valueToClsType(type);
    bool isAffects =
        clsHelper.chooseDeType2(clsType) == DesignEntityType::ASSIGN;
    bool isStmtStmt =
        clsHelper.chooseDeType2(clsType) == DesignEntityType::STMT;
    bool isProcProc =
        clsHelper.chooseDeType2(clsType) == DesignEntityType::PROCEDURE;
    if (isAffects) {
        return parseAffects();
    } else if (isStmtStmt) {
        return parseStmtStmt();
    } else if (isProcProc) {
        return parseProcProc();
    } else {
        return parseXEnt();
    }
}

/**
 * Parses a clause that takes in (stmt, stmt) as parameters.
 * This includes: Follows(*)/Parent(*)/Next(*).
 * @return Clause object.
 * @exception ValidityError if invalid arguments.
 */
Clause *SuchThatParser::parseStmtStmt() {
    // integer/synonym/wildcard, integer/synonym/wildcard

    ClauseType clsType = clsHelper.valueToClsType(this->type);

    r1 = parseStmtRef(this->ref1);
    r2 = parseStmtRef(this->ref2);
    if (r1 == nullptr || r2 == nullptr) {
        throw ValidityError("QP-ERROR: invalid clause argument");
    }

    return new Clause(clsType, *r1, *r2);
}

/**
 * Parses a clause that takes in (assign, assign) as parameters.
 * This includes: Affects(*).
 * @return Clause object.
 * @exception ValidityError if invalid arguments.
 */
Clause *SuchThatParser::parseAffects() {
    // integer/assign/wildcard, integer/assign/wildcard

    ClauseType clsType = clsHelper.valueToClsType(this->type);

    r1 = parseAssignRef(this->ref1);
    r2 = parseAssignRef(this->ref2);
    if (r1 == nullptr || r2 == nullptr) {
        throw ValidityError("QP-ERROR: invalid clause argument");
    }

    return new Clause(clsType, *r1, *r2);
}

/**
 * Parses a clause that takes in (procedure, procedure) as parameters.
 * This includes: Calls(*).
 * @return Clause object.
 * @exception ValidityError if invalid arguments.
 */
Clause *SuchThatParser::parseProcProc() {
    // quoted/synonym/wildcard, quoted/synonym/wildcard

    ClauseType clsType = clsHelper.valueToClsType(this->type);

    r1 = parseEntRef(this->ref1, DesignEntityType::PROCEDURE);
    r2 = parseEntRef(this->ref2, DesignEntityType::PROCEDURE);
    if (r1 == nullptr || r2 == nullptr) {
        throw ValidityError("QP-ERROR: invalid clause argument");
    }

    return new Clause(clsType, *r1, *r2);
}

/**
 * Parses a clause that takes in (X, ent) as parameters.
 * This includes: Modifies/Uses (both P and S versions).
 * @return Clause object.
 * @exception ValidityError if invalid arguments.
 */
Clause *SuchThatParser::parseXEnt() {
    if (ParserUtil::isWildcard(this->ref1)) {
        throw ValidityError("QP-ERROR: first argument cannot be wildcard");
    }

    r2 = parseEntRef(this->ref2, DesignEntityType::VARIABLE);
    if (r2 == nullptr) {
        throw ValidityError("QP-ERROR: invalid clause argument");
    }

    if (type == "Modifies") {
        return parseModifies();
    } else if (type == "Uses") {
        return parseUses();
    } else {
        throw ValidityError("QP-ERROR: invalid clause type");
    }
}

/**
 * Parses a Modifies clause that takes in (X, end) as parameters.
 * This includes: ModifiesP and ModifiesS.
 * @return Clause object.
 * @exception ValidityError if invalid arguments.
 */
Clause *SuchThatParser::parseModifies() {
    // MODIFIES_S: integer/synonym, quoted/variable/wildcard
    // MODIFIES_P: quoted/synonym, quoted/variable/wildcard
    ClauseType clsType;

    Reference *r1Stmt = parseStmtRef(this->ref1);
    Reference *r1Proc = parseEntRef(this->ref1, DesignEntityType::PROCEDURE);
    if (r1Stmt == nullptr && r1Proc == nullptr) {
        throw ValidityError("QP-ERROR: invalid clause argument");
    }
    if (r1Stmt != nullptr) {
        if (r1Stmt->getDeType() == DesignEntityType::PRINT) {
            delete r1Stmt;
            throw ValidityError("QP-ERROR: invalid clause argument");
        }
        r1 = r1Stmt;
        clsType = ClauseType::MODIFIES_S;
    } else {
        r1 = r1Proc;
        clsType = ClauseType::MODIFIES_P;
    }

    return new Clause(clsType, *r1, *r2);
}

/**
 * Parses a Uses clause that takes in (X, end) as parameters.
 * This includes: UsesP and UsesS.
 * @return Clause object.
 * @exception ValidityError if invalid arguments.
 */
Clause *SuchThatParser::parseUses() {
    // USES_S: integer/synonym, quoted/variable/wildcard
    // USES_P: quoted/synonym, quoted/variable/wildcard
    ClauseType clsType;

    Reference *r1Stmt = parseStmtRef(this->ref1);
    Reference *r1Proc = parseEntRef(this->ref1, DesignEntityType::PROCEDURE);
    if (r1Stmt == nullptr && r1Proc == nullptr) {
        throw ValidityError("QP-ERROR: invalid clause argument");
    }
    if (r1Stmt != nullptr) {
        if (r1Stmt->getDeType() == DesignEntityType::READ) {
            delete r1Stmt;
            throw ValidityError("QP-ERROR: invalid clause argument");
        }
        r1 = r1Stmt;
        clsType = ClauseType::USES_S;
    } else {
        r1 = r1Proc;
        clsType = ClauseType::USES_P;
    }

    return new Clause(clsType, *r1, *r2);
}

/**
 * Parses a `assignRef` string into a `Reference` object.
 * This includes: stmt, assign, prog_line.
 * @param syn The assignRef.
 * @return Reference object, otherwise nullptr.
 */
Reference *SuchThatParser::parseAssignRef(string syn) {
    // syn:  ASSIGN | INTEGER | WILDCARD
    if (ParserUtil::isQuoted(syn)) {
        return nullptr;
    }

    Reference *r = getReferenceIfDeclared(syn);
    if (r != nullptr) {
        DesignEntityType deType = r->getDeType();
        if (deType != DesignEntityType::ASSIGN &&
            deType != DesignEntityType::PROG_LINE &&
            deType != DesignEntityType::STMT) {
            return nullptr;
        }

        if (deType == DesignEntityType::PROG_LINE) {
            deType = DesignEntityType::STMT;
        }
        return new Reference(deType, r->getRefType(), syn, r->getAttr());
    }

    DesignEntityType deType = DesignEntityType::STMT;
    ReferenceType refType = ParserUtil::parseRefType(syn);
    ReferenceAttribute attr = ReferenceAttribute::INTEGER;
    return new Reference(deType, refType, syn, attr);
}
