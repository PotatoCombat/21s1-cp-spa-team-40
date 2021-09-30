#include "query_processor/parser/PatternParser.h"

PatternParser::PatternParser() {
    this->ident = "";
    this->args.clear();
}

void PatternParser::initReferences(vector<Reference *> &declList) {
    this->declList = declList;
}

void PatternParser::clear() {
    this->declList.clear();
    this->ident = "";
    this->args.clear();
}

PatternClause *PatternParser::parse(PatTuple patTuple) {
    this->ident = patTuple.first;
    this->args = patTuple.second;
    // first Reference type variable, others strings ?

    Reference *identity = getReferenceIfDeclared(this->ident);

    if (isAssignPattern(identity)) {
        return parseAssign(identity);
    } else {
        throw ValidityError("invalid pattern clause");
    }
}

PatternClause *PatternParser::parseAssign(Reference *identity) {

    //if (it != declList.end()) {
    //    if ((*it)->getDeType() == DesignEntityType::ASSIGN) {
    //        r = (*it)->copy();
    //    } else {
    //        throw ValidityError("invalid symbol");
    //    }
    //} else {
    //    throw ValidityError("undeclared synonym");
    //}

    //if (it1 != declList.end()) {
    //    r1 = (*it1)->copy();
    //} else {
    //    // TODO: assert quoted
    //    ReferenceType refT = ParserUtil::checkRefType(LHS);
    //    DesignEntityType deT = DesignEntityType::VARIABLE;
    //    if (ParserUtil::isQuoted(LHS)) {
    //        LHS = LHS.substr(1, LHS.size() - 2);
    //    }
    //    r1 = new Reference(deT, refT, LHS);
    //}

    //return new PatternClause(*r, *r1, RHS);
}

bool PatternParser::isAssignPattern(Reference *identity) {
    return identity->getDeType() == DesignEntityType::ASSIGN &&
           this->args.size() == 2;
}

// bool isWhilePattern(Reference *identity) {
//    return identity->getDeType() == DesignEntityType::ASSIGN &&
//           this->args.size() == 3;
//}
//
// bool isIfPattern(Reference *identity) {
//    return identity->getDeType() == DesignEntityType::ASSIGN &&
//           this->args.size() == 3;
//}

/**
 * Retrieves synonym in the declaration list if it exists.
 * @param syn The synonym name.
 * @return Reference object if match, otherwise nullptr.
 */
Reference *PatternParser::getReferenceIfDeclared(string syn) {
    auto it = find_if(declList.begin(), declList.end(), [&syn](Reference *ref) {
        return ref->getValue() == syn;
    });

    if (it != declList.end()) {
        return *it;
    }
    return nullptr;
}
