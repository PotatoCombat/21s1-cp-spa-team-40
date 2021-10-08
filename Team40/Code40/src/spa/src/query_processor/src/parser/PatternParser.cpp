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
    // first arg is reference (variable-type)
    // remaining args are strings

    Reference *identity = getReferenceIfDeclared(this->ident);

    if (identity == nullptr) {
        throw ValidityError("undeclared pattern synonym");
    }

    identity = identity->copy();

    if (isAssignPattern(identity)) {
        return parseAssign(identity);
    } else if (isWhilePattern(identity)) {
        return parseWhile(identity);
    } else if (isIfPattern(identity)) {
        return parseIf(identity);
    } else {
        delete identity;
        throw ValidityError("invalid pattern type");
    }
}

PatternClause *PatternParser::parseAssign(Reference *identity) {
    string var = this->args.at(0);
    string pattern = this->args.at(1);
    Reference *ref = parseValidVariable(var);

    bool isExactMatch = isExactPattern(pattern);

    // temp change to make build success
    return new PatternClause(*identity, *ref, pattern, true);
}

PatternClause *PatternParser::parseWhile(Reference *identity) {
    string var = this->args.at(0);
    Reference *ref = parseValidVariable(var);

    if (!ParserUtil::isWildcard(this->args.at(1))) {
        throw ValidityError("while clause 2nd argument should be _");
    }
    return new PatternClause(*identity, *ref);
}

PatternClause *PatternParser::parseIf(Reference *identity) {
    string var = this->args.at(0);
    Reference *ref = parseValidVariable(var);

    if (!ParserUtil::isWildcard(this->args.at(1)) ||
        !ParserUtil::isWildcard(this->args.at(2))) {
        throw ValidityError("if clause 2nd & 3rd arguments should be _");
    }
    return new PatternClause(*identity, *ref);
}

Reference *PatternParser::parseValidVariable(string var) {
    Reference *ref = getReferenceIfDeclared(var);

    if (ref != nullptr) {
        if (ref->getDeType() != DesignEntityType::VARIABLE) {
            throw ValidityError("invalid reference");
        }
        return ref->copy();
    }

    DesignEntityType deT = DesignEntityType::VARIABLE;
    ReferenceType refT;
    if (ParserUtil::isWildcard(var)) {
        refT = ReferenceType::WILDCARD;
    } else if (ParserUtil::isQuoted(var)) {
        refT = ReferenceType::CONSTANT;
        var = var.substr(1, var.size() - 2);
    } else {
        throw ValidityError("invalid reference");
    }
    return new Reference(deT, refT, var);
}

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

bool PatternParser::isAssignPattern(Reference *identity) {
    return identity->getDeType() == DesignEntityType::ASSIGN &&
           this->args.size() == 2;
}

bool PatternParser::isWhilePattern(Reference *identity) {
    return identity->getDeType() == DesignEntityType::WHILE &&
           this->args.size() == 2;
}

bool PatternParser::isIfPattern(Reference *identity) {
    return identity->getDeType() == DesignEntityType::IF &&
           this->args.size() == 3;
}

bool PatternParser::isExactPattern(string pattern) {
    if (ParserUtil::isWildcard(pattern) || ParserUtil::isQuoted(pattern)) {
        return true;
    } else if (ParserUtil::isUnderscoredQuoted(pattern)) {
        return false;
    }
    throw ValidityError("invalid pattern string");
}
