#include "query_processor/parser/StmtStmtParser.h"

StmtStmtParser::StmtStmtParser(ClsTuple cls, vector<Reference *> &declList,
                               DesignEntityTypeHelper &deH,
                               ClauseTypeHelper &clsH)
    : type(get<0>(cls)), ref1(get<1>(cls)), ref2(get<2>(cls)),
      declList(declList), deHelper(deH), clsHelper(clsH) {}

Clause *StmtStmtParser::parse() {
    // FOLLOWS/FOLLOWS_T/PARENT/PARENT_T
    // number/synonym/wildcard, number/synonym/wildcard

    string refString1 = this->ref1;
    string refString2 = this->ref2;

    // see if any declared synonyms used
    auto it1 = find_if(declList.begin(), declList.end(),
                       [&refString1](Reference *ref) {
                           return ref->getValue() == refString1;
                       });
    auto it2 = find_if(declList.begin(), declList.end(),
                       [&refString2](Reference *ref) {
                           return ref->getValue() == refString2;
                       });

    ClauseType clsT = clsHelper.getType(type);
    Reference *r1;
    Reference *r2;

    // both r1 and r2 must be statements 
    if (it1 != declList.end()) {
        DesignEntityType foundType = (*it1)->getDeType();
        if (!deHelper.isStatement(foundType)) {
            throw ValidityError("invalid clause argument");
        }
        r1 = (*it1)->copy();
    } else {
        // statement is a constant/wildcard
        // TODO: assert integer / wildcard
        ReferenceType refT = ParserUtil::checkRefType(ref1);
        DesignEntityType deT = clsHelper.chooseDeType1(clsT);
        r1 = new Reference(deT, refT, ref1);
    }

    if (it2 != declList.end()) {
        DesignEntityType foundType = (*it2)->getDeType();
        if (!deHelper.isStatement(foundType)) {
            throw ValidityError("invalid clause argument");
        }
        r2 = (*it2)->copy();
    } else {
        // statement is a constant/wildcard
        // TODO: assert integer / wildcard
        ReferenceType refT = ParserUtil::checkRefType(ref2);
        DesignEntityType deT = clsHelper.chooseDeType2(clsT);
        r2 = new Reference(deT, refT, ref2);
    }

    return new Clause(clsT, *r1, *r2);
}
