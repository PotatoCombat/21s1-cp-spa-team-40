#include "query_processor/parser/StmtEntParser.h"

StmtEntParser::StmtEntParser(ClsTuple cls, vector<Reference *> &declList,
                             DesignEntityTypeHelper &deH,
                             ClauseTypeHelper &clsH)
    : type(get<0> cls), ref1(get<1> cls), ref2(get<1> cls), declList(declList),
      deHelper(deH), clsHelper(clsH) {}

Clause *StmtEntParser::parse() {
    // MODIFIES_S/USES_S
    // number/synonym, quoted/synonym/wildcard

    auto it1 =
        find_if(declList.begin(), declList.end(),
                [&ref1](Reference *ref) { return ref->getValue() == ref1; });
    auto it2 =
        find_if(declList.begin(), declList.end(),
                [&ref2](Reference *ref) { return ref->getValue() == ref2; });

    ClauseType clsT = clsHelper.getType(type);
    Reference *r1;
    Reference *r2;

    if (it1 != declList.end()) {
        DesignEntityType foundType = (*it1)->getDeType();
        if (!deHelper.isStatement(foundType)) {
            throw ValidityError("invalid clause argument");
        }
        r1 = (*it1)->copy();
    } else {
        ReferenceType refT = checkRefType(ref1); // TODO: assert integer
        DesignEntityType deT = clsHelper.chooseDeType1(clsT);
        r1 = new Reference(deT, refT, ref1);
    }

    if (it2 != declList.end()) {
        DesignEntityType foundType = (*it2)->getDeType();
        if (!deHelper.isVariable(foundType)) {
            throw ValidityError("invalid clause argument");
        }
        r2 = (*it2)->copy();
    } else {
        ReferenceType refT =
            checkRefType(ref2); // TODO: assert quoted / wildcard
        DesignEntityType deT = clsHelper.chooseDeType2(clsT);
        if (refT == ReferenceType::CONSTANT) {
            ref2 = ref2.substr(1, ref2.size() - 2);
        }
        r2 = new Reference(deT, refT, ref2);
    }

    return new Clause(clsT, *r1, *r2);
}
