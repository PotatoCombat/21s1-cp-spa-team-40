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

    if (identity == nullptr) {
        throw ValidityError("undeclared pattern synonym");
    }

    identity = identity->copy();

    if (isAssignPattern(identity)) {
        return parseAssign(identity);
    } else {
        delete identity;
        throw ValidityError("invalid pattern type");
    }
}

PatternClause *PatternParser::parseAssign(Reference *identity) {
    string var = this->args.at(0);
    string pattern = this->args.at(1);
    Reference *ref = getReferenceIfDeclared(var);

    if (ref != nullptr) {
        ref = ref->copy();
    } else {
        // TODO: assert quoted or wildcard
        DesignEntityType deT = DesignEntityType::VARIABLE;
        ReferenceType refT = ParserUtil::checkRefType(var);
        if (ParserUtil::isQuoted(var)) {
            var = var.substr(1, var.size() - 2);
        }
        ref = new Reference(deT, refT, var);
    }

    return new PatternClause(*identity, *ref, pattern, true); // temp change to make build success
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
