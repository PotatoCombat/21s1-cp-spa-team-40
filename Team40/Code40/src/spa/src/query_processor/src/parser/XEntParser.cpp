#include "query_processor/parser/XEntParser.h"

XEntParser::XEntParser(ClsTuple cls, vector<Reference *> &declList,
                       DesignEntityTypeHelper &deH, ClauseTypeHelper &clsH)
    : type(get<0>(cls)), ref1(get<1>(cls)), ref2(get<2>(cls)),
      declList(declList), deHelper(deH), clsHelper(clsH) {}

Clause *XEntParser::parse() {
    // MODIFIES_S/USES_S
    // number/synonym, quoted/synonym/wildcard

    // MODIFIES_P/USES_P
    // quoted/synonym, quoted/synonym/wildcard

    if (ParserUtil::isWildcard(ref1)) {
        throw ValidityError("first argument cannot be wildcard");
    }

    string refString1 = this->ref1;
    string refString2 = this->ref2;

    // check if references are declared
    auto it1 = find_if(declList.begin(), declList.end(),
                       [&refString1](Reference *ref) {
                           return ref->getValue() == refString1;
                       });
    auto it2 = find_if(declList.begin(), declList.end(),
                       [&refString2](Reference *ref) {
                           return ref->getValue() == refString2;
                       });

    Reference *r1;
    Reference *r2;
    // a StmtEnt is ModifiesS/UsesS
    // a EntEnt is ModifiesP/UsesP
    bool isStmtEnt = true; // false for EntEnt

    if (it1 != declList.end()) {
        DesignEntityType foundType = (*it1)->getDeType();
        // set isStmtEnt based on the type of first argument
        if (deHelper.isStatement(foundType)) {
            isStmtEnt = true;
        } else if (deHelper.isProcedure(foundType)) {
            isStmtEnt = false;
        } else {
            throw ValidityError("invalid clause argument");
        }
        r1 = (*it1)->copy();
    } else {
        // first argument is not declared, so must be either constant or wildcard
        DesignEntityType deT;
        if (ParserUtil::isInteger(ref1)) { // integer constant
            isStmtEnt = true;
            deT = DesignEntityType::STMT;
        } else if (ParserUtil::isQuoted(ref1)) { // string constant
            isStmtEnt = false;
            deT = DesignEntityType::PROCEDURE;
            ref1 = ref1.substr(1, ref1.size() - 2); // remove quotes
        } else {
            throw ValidityError("invalid clause argument");
        }
        ReferenceType refT = ReferenceType::CONSTANT;
        r1 = new Reference(deT, refT, ref1);
    }

    // second argument must always be a variable
    if (it2 != declList.end()) {
        DesignEntityType foundType = (*it2)->getDeType();
        if (!deHelper.isVariable(foundType)) {
            throw ValidityError("invalid clause argument");
        }
        r2 = (*it2)->copy();
    } else {
        ReferenceType refT =
            ParserUtil::checkRefType(ref2); // TODO: assert quoted / wildcard
        if (refT == ReferenceType::CONSTANT) { // remove quotes if constant
            ref2 = ref2.substr(1, ref2.size() - 2);
        }
        DesignEntityType deT = DesignEntityType::VARIABLE;
        r2 = new Reference(deT, refT, ref2);
    }

    if (isStmtEnt) {
        // make ModifiesS/UsesS
        return new Clause(clsHelper.valueToClsType(type), *r1, *r2);
    }
    // make ModifiesP/UsesP
    return new Clause(clsHelper.valueToClsType(type + "*"), *r1, *r2);
}
