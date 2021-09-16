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
    vector<PatArg> patArgs = get<1>(pattern);
    string LHS = patArgs[0];
    string RHS = patArgs[1];

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
        r = (*it)->copy();
    } else {
        throw ValidityError("undeclared synonym");
    }

    if (it1 != declList.end()) {
        r1 = (*it1)->copy();
    } else {
        ReferenceType refT =
            ParserUtil::checkRefType(LHS); // TODO: assert quoted
        DesignEntityType deT = DesignEntityType::VARIABLE;
        r1 = new Reference(deT, refT, LHS);
    }

    return new PatternClause(*r, *r1, RHS);
}
