#include "query_processor/parser/XEntParser.h"

XEntParser::XEntParser(ClsTuple cls, vector<Reference *> &declList,
                       DesignEntityTypeHelper &deH, ClauseTypeHelper &clsH)
    : type(get<0> cls), ref1(get<1> cls), ref2(get<1> cls), declList(declList),
      deHelper(deH), clsHelper(clsH) {}

Clause *XEntParser::parse() {
    // MODIFIES_S/USES_S
    // number/synonym, quoted/synonym/wildcard

    // MODIFIES_P/USES_P
    // quoted/synonym, quoted/synonym/wildcard

    // number/synonym, quoted/synonym/wildcard
    if (isQuoted(ref1) || isWildcard(ref1) || isInteger(ref2)) {
        throw ValidityError("invalid clause argument");
    }

    // quoted/synonym, quoted/synonym/wildcard
    if (isInteger(ref1) || isWildcard(ref1) || isInteger(ref2)) {
        throw ValidityError("invalid clause argument");
    }

    auto it1 =
        find_if(declList.begin(), declList.end(),
                [&ref1](Reference *ref) { return ref->getValue() == ref1; });
    auto it2 =
        find_if(declList.begin(), declList.end(),
                [&ref2](Reference *ref) { return ref->getValue() == ref2; });

    ClauseType clsT = clsHelper.getType(type);
    Reference *r1;
    Reference *r2;
    bool isStmtEnt = true; // false for EntEnt

    if (it1 != declList.end()) {
        DesignEntityType foundType = (*it1)->getDeType();
        if (deHelper.isStatement(foundType)) {
            isStmtEnt = true;
        } else if (deHelp.isProcedure(foundType)) {
            isStmtEnt = false;
        } else {
            throw ValidityError("invalid clause argument");
        }
        r1 = (*it1)->copy();
    } else {
        DesignEntityType deT;
        if (ParserUtil::isInteger(ref1)) {
            isStmtEnt = true;
            deT = DesignEntityType::STMT;
        } else if (ParserUtil::isQuoted(ref1)) {
            isStmtEnt = false;
            deT = DesignEntityType::PROCEDURE;
        } else {
            throw ValidityError("invalid clause argument");
        }
        r1 = new Reference(deT, refT, ref1);
    }

    if (it2 != declList.end()) {
        DesignEntityType foundType = (*it2)->getDeType();
        if (!deHelper.isVariable(foundType)) {
            throw ValidityError("invalid clause argument");
        }
        r2 = (*it2)->copy();
    } else {
        ReferenceType refT = checkRefType(ref2); // TODO: assert quoted / wildcard
        if (refT != ReferenceType::CONSTANT) {
            throw ValidityError("invalid clause argument");
        }
        ref2 = ref2.substr(1, ref2.size() - 2);
        DesignEntityType deT = DesignEntityType::VARIABLE;
        r2 = new Reference(deT, refT, ref2);
    }

    if (isStmtEnt) {
        return new Clause(clsHelper.getType(type), *r1, *r2);
    }
    return new Clause(clsHelper.getType(type + "*"), *r1, *r2);
}
