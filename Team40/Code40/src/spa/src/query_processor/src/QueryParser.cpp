#include "query_processor/QueryParser.h"

Reference *QueryParser::parseDeclaration(DeclPair declaration) {
    DesignEntityType type = deHelper.getType(declaration.first);
    string syn = declaration.second;
    return new Reference(type, ReferenceType::SYNONYM, syn);
}

Clause *QueryParser::parseClause(ClsTuple clause,
                                 vector<Reference *> &declList) {
    string cls = get<0>(clause);
    string ref1 = get<1>(clause);
    string ref2 = get<2>(clause);

    bool isStmtStmt = clsHelper.chooseDeType2(clsHelper.getType(cls)) ==
                      DesignEntityType::STMT;
    if (isStmtStmt) {
        // number/synonym/wildcard, number/synonym/wildcard
        if (ParserUtil::isQuoted(ref1) || ParserUtil::isQuoted(ref2)) {
            throw ValidityError("invalid clause argument");
        }
        StmtStmtParser p(clause, declList, deHelper, clsHelper);
        return p.parse();
    }

    XEntParser p(clause, declList, deHelper, clsHelper);
    return p.parse();
}

PatternClause *QueryParser::parsePattern(PatTuple pattern,
                                         vector<Reference *> &declList) {
    string identity = get<0>(pattern);
    PatArg LHS = get<1>(pattern);
    PatArg RHS = get<2>(pattern);

    Reference *r;
    Reference *r1;
    auto it =
        find_if(declList.begin(), declList.end(), [&identity](Reference *ref) {
            return ref->getValue() == identity;
        });
    auto it1 =
        find_if(declList.begin(), declList.end(),
                [&LHS](Reference *ref) { return ref->getValue() == LHS; });

    if (it != declList.end()) {
        if ((*it)->getDeType() == DesignEntityType::ASSIGN) {
            r = (*it)->copy();
        } else {
            throw ValidityError("invalid symbol");
        }
    } else {
        throw ValidityError("undeclared synonym");
    }

    if (it1 != declList.end()) {
        r1 = (*it1)->copy();
    } else {
        // TODO: assert quoted
        ReferenceType refT = ParserUtil::checkRefType(LHS);
        DesignEntityType deT = DesignEntityType::VARIABLE;
        if (ParserUtil::isQuoted(LHS)) {
            LHS = LHS.substr(1, LHS.size() - 2);
        }
        r1 = new Reference(deT, refT, LHS);
    }

    return new PatternClause(*r, *r1, RHS);
}
